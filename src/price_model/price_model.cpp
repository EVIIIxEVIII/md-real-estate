#include "price_model.hpp"
#include <Eigen/src/Core/Matrix.h>
#include <csv.h>
#include <iostream>
#include <fstream>

namespace price_model {

PriceModel::PriceModel(std::string data_path): data_path(data_path) {
    LoadedData data = load_data(data_path);
    SplitData spl_data = split_data(data);

    enlr(spl_data.X_train, spl_data.Y_train, 2.0, 3.0);
    evaluate_model(spl_data.x_test, spl_data.y_test);
}

SplitData PriceModel::split_data(LoadedData<PriceModel::F> data)
{
    const std::size_t training_set_size = std::round(data.X_rows.size() * .75);
    const std::size_t test_set_size = data.X_rows.size() - training_set_size;

    assert(training_set_size && "empty CSV?");

    Eigen::MatrixXd X_train(training_set_size, F + 1);
    Eigen::VectorXd Y_train = Eigen::VectorXd(training_set_size);
    for (std::size_t i = 0; i < training_set_size; ++i) {
        X_train(i, 0) = 1.0;
        for (std::size_t j = 0; j < F; ++j)
            X_train(i, j + 1) = data.X_rows[i][j];

        Y_train(i) = data.y_vals[i];
    }

    Eigen::MatrixXd X_test(test_set_size, F + 1);
    Eigen::VectorXd Y_test = Eigen::VectorXd(test_set_size);
    for (std::size_t i = 0; i < test_set_size; ++i) {
        int dataset_index = training_set_size + i;

        X_test(i, 0) = 1.0;
        for (std::size_t j = 0; j < F; ++j)
            X_test(i, j + 1) = data.X_rows[dataset_index][j];

        Y_test(i) = data.y_vals[dataset_index];
    }

    return SplitData {
        X_train,
        Y_train,
        X_test,
        Y_test
    };
}

template<std::size_t... Is>
bool PriceModel::read_row_array(io::CSVReader<F + 1>& in,
                    std::array<double, F>&  x,
                    double&                 y,
                    std::index_sequence<Is...>)
{
    return in.read_row(x[Is]..., y);
}

LoadedData<PriceModel::F> PriceModel::load_data(std::string data_path) {
    io::CSVReader<F + 1> in(data_path);

    [&]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        in.read_header(io::ignore_extra_column, feature[Is]..., "price");
    }(std::make_index_sequence<F>{});

    using Row = std::array<double, F>;
    std::vector<Row> X_rows;
    std::vector<double>  y_vals;

    Row    x{};
    double y = 0.;

    while (read_row_array(in, x, y, std::make_index_sequence<F>{})) {
        X_rows.push_back(x);
        y_vals.push_back(y);
    }

    std::array<double, F> means{};
	std::array<double, F> stds{};

	for (std::size_t col : normalized_features) {
		double sum = 0.0;
		for (const auto& row : X_rows)
			sum += row[col];
		means[col] = sum / X_rows.size();

		double var = 0.0;
		for (const auto& row : X_rows)
			var += (row[col] - means[col]) * (row[col] - means[col]);
		stds[col] = std::sqrt(var / X_rows.size());
	}

	for (auto& row : X_rows) {
		for (std::size_t col : normalized_features) {
			row[col] = (row[col] - means[col]) / stds[col];
		}
	}

    return LoadedData<F> {
        X_rows,
        y_vals
    };
}

double PriceModel::soft_threshold(double z, double gamma) {
    if (z > gamma) return z - gamma;
    if (z < -gamma) return z + gamma;
    return 0.0;
}

void PriceModel::save_vector_binary(const std::string& filename, const Eigen::VectorXd& vec) {
	std::ofstream out(filename, std::ios::binary);
	if (!out) {
		throw std::runtime_error("Cannot open file for writing: " + filename);
	}

	const Eigen::Index size = vec.size();
	out.write(reinterpret_cast<const char*>(&size), sizeof(Eigen::Index));
	out.write(reinterpret_cast<const char*>(vec.data()), sizeof(double) * size);
}

void PriceModel::enlr(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, double lambda1, double lambda2) {
    const int max_iter = 1000;
    const double tol = 1e-6;

    Eigen::VectorXd beta = Eigen::VectorXd::Ones(F + 1);
    Eigen::VectorXd beta_old = beta;

    for (int iter = 0; iter < max_iter; ++iter) {
        for (int j = 0; j < F + 1; j++) {
            Eigen::VectorXd X_j = X.col(j);
            double rho_j = X_j.dot(y - X*beta + beta(j)*X_j);
            beta(j) = soft_threshold(rho_j, lambda1) / (X_j.squaredNorm() + lambda2);
        }

        if ((beta - beta_old).norm() < tol) break;

        beta_old = beta;
    }

    model = beta;
}

inline void PriceModel::ols(const Eigen::MatrixXd& X, const Eigen::VectorXd& y)
{
	const double lambda = 3.0;

	const std::size_t n_features = X.cols();
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n_features, n_features);
	I(0, 0) = 0.0;

	model = (X.transpose() * X + lambda * I).ldlt().solve(X.transpose() * y);
}

void PriceModel::evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test)
{
	Eigen::VectorXd y_pred = x_test * model;
	assert(y_test.size() == y_pred.size());

	double mae = 0.0;
	double mse = 0.0;
	double ss_res = 0.0;
	double ss_tot = 0.0;
	double y_mean = y_test.mean();

	for (int i = 0; i < y_test.size(); ++i) {
		double err = y_test(i) - y_pred(i);
		mae += std::abs(err);
		mse += err * err;
		ss_res += err * err;
		ss_tot += std::pow(y_test(i) - y_mean, 2);
	}

	mae /= y_test.size();
	mse /= y_test.size();
	double rmse = std::sqrt(mse);
	double r2 = 1.0 - (ss_res / ss_tot);

	//std::cout << "True | Predicted\n";
	//for (int i = 0; i < y_test.size(); ++i)
	//	std::cout << y_test(i) << " " << y_pred(i) << "\n";

	std::cout << "\nEvaluation Metrics:\n";
	std::cout << "MAE  = " << mae << "\n";
	std::cout << "MSE  = " << mse << "\n";
	std::cout << "RMSE = " << rmse << "\n";
	std::cout << "R^2  = " << r2 << "\n";
}

}

