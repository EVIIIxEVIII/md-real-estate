#include "price_model_ols.hpp"
#include <Eigen/src/Core/Matrix.h>
#include <csv.h>
#include <iostream>
#include <fstream>

namespace price_model {

Ols::Ols(std::shared_ptr<DataHandler> data_handler): data_handler(data_handler) {
    SplitData spl_data = data_handler->get_split_data(.75, true);

    ols(spl_data.X_train, spl_data.Y_train);
    evaluate_model(spl_data.x_test, spl_data.y_test);

    save_vector_binary("./models/ols.bin");
}

double Ols::soft_threshold(double z, double gamma) {
    if (z > gamma) return z - gamma;
    if (z < -gamma) return z + gamma;
    return 0.0;
}

void Ols::save_vector_binary(const std::string& filename) {
	std::ofstream out(filename, std::ios::binary);
	if (!out) {
		throw std::runtime_error("Cannot open file for writing: " + filename);
	}

	const Eigen::Index size = model.size();
	out.write(reinterpret_cast<const char*>(&size), sizeof(Eigen::Index));
	out.write(reinterpret_cast<const char*>(model.data()), sizeof(double) * size);
}

void Ols::enlr(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, double lambda1, double lambda2) {
    const int max_iter = 1000;
    const double tol = 1e-6;

    Eigen::VectorXd beta = Eigen::VectorXd::Ones(DataHandler::F + 1);
    Eigen::VectorXd beta_old = beta;

    for (int iter = 0; iter < max_iter; ++iter) {
        for (int j = 0; j < DataHandler::F + 1; j++) {
            Eigen::VectorXd X_j = X.col(j);
            double rho_j = X_j.dot(y - X*beta + beta(j)*X_j);
            beta(j) = soft_threshold(rho_j, lambda1) / (X_j.squaredNorm() + lambda2);
        }

        if ((beta - beta_old).norm() < tol) break;

        beta_old = beta;
    }

    model = beta;
}

inline void Ols::ols(const Eigen::MatrixXd& X, const Eigen::VectorXd& y)
{
	const double lambda = 3.0;

	const std::size_t n_features = X.cols();
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n_features, n_features);
	I(0, 0) = 0.0;

	model = (X.transpose() * X + lambda * I).ldlt().solve(X.transpose() * y);
}

void Ols::evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test)
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

