#include "price_model_xgboost.hpp"

#include <Eigen/Dense>
#include <iostream>

namespace price_model {

void XGBoost::evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test) {
	Eigen::VectorXd y_pred = predict(x_test);
	assert(y_test.size() == y_pred.size());

	double mae = 0.0;
	double mse = 0.0;
	double ss_res = 0.0;
	double ss_tot = 0.0;
	double rmsle_sum = 0.0;
	double y_mean = y_test.mean();

	for (int i = 0; i < y_test.size(); ++i) {
		double pred = std::max(y_pred(i), 1.0);
		double actual = std::max(y_test(i), 1.0);

		double err = actual - pred;
		mae += std::abs(err);
		mse += err * err;
		ss_res += err * err;
		ss_tot += std::pow(actual - y_mean, 2);

		double log_err = std::log1p(pred) - std::log1p(actual);
		rmsle_sum += log_err * log_err;
	}

	int n = y_test.size();
	mae /= n;
	mse /= n;
	double rmse = std::sqrt(mse);
	double r2 = 1.0 - (ss_res / ss_tot);
	double rmsle = std::sqrt(rmsle_sum / n);

	std::cout << "\nEvaluation Metrics:\n";
	std::cout << "MAE   = " << mae << "\n";
	std::cout << "MSE   = " << mse << "\n";
	std::cout << "RMSE  = " << rmse << "\n";
	std::cout << "R^2   = " << r2 << "\n";
	std::cout << "RMSLE = " << rmsle << "\n";
}

Eigen::VectorXd XGBoost::predict(const Eigen::MatrixXd& X) {
    const int trees_num = model.xgboost_trees.size();
    const double learning_rate = config.eta;

    Eigen::VectorXd y_pred = Eigen::VectorXd::Constant(X.rows(), model.initial);

    for (const auto& tree : model.xgboost_trees) {
		y_pred += learning_rate * tree->predict(X);
	}

    return y_pred;
}

void XGBoost::train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y) {
    assert((y.size() != 0) && "y cannot be empty!");
    assert((y.size() == X.rows()) && "The number of rows for X and y do not match!");

    double initial = y.mean();
    Eigen::VectorXd predictions = Eigen::VectorXd::Constant(y.size(), initial);
    Eigen::VectorXd residues = y - predictions;

    std::vector<std::unique_ptr<XGBoostTree>> xgboost_trees;
    xgboost_trees.reserve(config.n_estimators);

    const double learning_rate = config.eta;
    const int n_rounds = config.n_estimators;

    for (int i = 0; i < n_rounds; i++) {
        auto tree = std::make_unique<XGBoostTree>(config.tree_config);
        tree->train(X, residues);
        Eigen::VectorXd tree_preds = tree->predict(X);

        predictions += learning_rate * tree_preds;
        residues = y - predictions;

        xgboost_trees.push_back(std::move(tree));
    }

    model = { initial, std::move(xgboost_trees) };
}

}
