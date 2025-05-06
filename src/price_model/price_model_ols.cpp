#include "price_model_ols.hpp"
#include "data_handler.hpp"

#include <Eigen/src/Core/Matrix.h>
#include <csv.h>
#include <iostream>

namespace price_model {

double Ols::soft_threshold(double z, double gamma) {
    if (z > gamma) return z - gamma;
    if (z < -gamma) return z + gamma;
    return 0.0;
}

Eigen::MatrixXd addIntercept(const Eigen::MatrixXd& X) {
	Eigen::MatrixXd X_aug(X.rows(), X.cols() + 1);
	X_aug.col(0) = Eigen::VectorXd::Ones(X.rows());
	X_aug.rightCols(X.cols()) = X;
	return X_aug;
}

Eigen::VectorXd Ols::predict(const Eigen::MatrixXd& X) {
    const Eigen::MatrixXd X_int = addIntercept(X);
    return X_int * model;
}

void Ols::enlr(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, double lambda1, double lambda2) {
    const Eigen::MatrixXd X_int = addIntercept(X);
    const int max_iter = 1000;
    const double tol = 1e-6;

    Eigen::VectorXd beta = Eigen::VectorXd::Ones(DataHandler::F + 1);
    Eigen::VectorXd beta_old = beta;

    for (int iter = 0; iter < max_iter; ++iter) {
        for (int j = 0; j < DataHandler::F + 1; j++) {
            Eigen::VectorXd X_j = X_int.col(j);
            double rho_j = X_j.dot(y - X_int*beta + beta(j)*X_j);
            beta(j) = soft_threshold(rho_j, lambda1) / (X_j.squaredNorm() + lambda2);
        }

        if ((beta - beta_old).norm() < tol) break;

        beta_old = beta;
    }

    model = beta;
}

inline void Ols::ols(const Eigen::MatrixXd& X, const Eigen::VectorXd& y)
{
    const Eigen::MatrixXd X_int = addIntercept(X);
	const double lambda = 3.0;

	const std::size_t n_features = X.cols();
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n_features, n_features);
	I(0, 0) = 0.0;

	model = (X_int.transpose() * X_int + lambda * I).ldlt().solve(X_int.transpose() * y);
}

void Ols::evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test)
{
    const Eigen::MatrixXd x_test_int = addIntercept(x_test);
	Eigen::VectorXd y_pred = x_test_int * model;
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

