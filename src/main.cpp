#include <array>
#include <memory>
#include <vector>
#include <cassert>
#include <iostream>
#include <csv.h>
#include <Eigen/Dense>
#include <cmath>
#include "./price_model/price_model.hpp"

template<std::size_t F, std::size_t... Is>
bool read_row_array(io::CSVReader<F + 1>& in,
                    std::array<double, F>&  x,
                    double&                 y,
                    std::index_sequence<Is...>)
{
    return in.read_row(x[Is]..., y);
}

inline Eigen::VectorXd ols(const Eigen::MatrixXd& X,
                           const Eigen::VectorXd& y)
{
    return X.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(y);
}

template<std::size_t F>
struct LoadedData {
    std::vector<std::array<double, F>> X_rows;
    std::vector<double>  y_vals;
};

struct Data {
    Eigen::MatrixXd X_train;
    Eigen::VectorXd Y_train;

    Eigen::MatrixXd x_test;
    Eigen::VectorXd y_test;
};

template<std::size_t F>
Data split_data(std::vector<std::array<double, F>> X_rows, std::vector<double>  y_vals, float train_data_ratio) {
    const std::size_t training_set_size = std::round(X_rows.size() * .75);
    const std::size_t test_set_size = X_rows.size() - training_set_size;

    assert(training_set_size && "empty CSV?");

    Eigen::MatrixXd X_train(training_set_size, F + 1);
    Eigen::VectorXd Y_train = Eigen::VectorXd(training_set_size);
    for (std::size_t i = 0; i < training_set_size; ++i) {
        X_train(i, 0) = 1.0;
        for (std::size_t j = 0; j < F; ++j)
            X_train(i, j + 1) = X_rows[i][j];

        Y_train(i) = y_vals[i];
    }

    Eigen::MatrixXd X_test(test_set_size, F + 1);
    Eigen::VectorXd Y_test = Eigen::VectorXd(test_set_size);
    for (std::size_t i = 0; i < test_set_size; ++i) {
        int dataset_index = training_set_size + i;

        X_test(i, 0) = 1.0;
        for (std::size_t j = 0; j < F; ++j)
            X_test(i, j + 1) = X_rows[dataset_index][j];

        Y_test(i) = y_vals[dataset_index];
    }

    return Data {
        X_train,
        Y_train,
        X_test,
        Y_test
    };
}

void evaluate_model(const Eigen::MatrixXd& x_test,
                    const Eigen::VectorXd& y_test,
                    const Eigen::VectorXd& model)
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

	std::cout << "True | Predicted\n";
	for (int i = 0; i < y_test.size(); ++i)
		std::cout << y_test(i) << " " << y_pred(i) << "\n";

	std::cout << "\nEvaluation Metrics:\n";
	std::cout << "MAE  = " << mae << "\n";
	std::cout << "MSE  = " << mse << "\n";
	std::cout << "RMSE = " << rmse << "\n";
	std::cout << "R^2  = " << r2 << "\n";
}

template<std::size_t F>
LoadedData<F> load_data(std::string data_path, std::array<const char*, F> feature) {
    io::CSVReader<F + 1> in(data_path);

    [&]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        in.read_header(io::ignore_extra_column, feature[Is]..., "price");
    }(std::make_index_sequence<F>{});

    using Row = std::array<double, F>;
    std::vector<std::array<double, F>> X_rows;
    std::vector<double>  y_vals;

    Row    x{};
    double y = 0.;

    while (read_row_array<F>(in, x, y, std::make_index_sequence<F>{})) {
        X_rows.push_back(x);
        y_vals.push_back(y);
    }

    return LoadedData<F> {
        X_rows,
        y_vals
    };
}


int main()
{
    auto price_model = std::make_unique<price_model::PriceModel>("./data/datasets/encoded_df.csv");

}
