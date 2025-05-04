#include "price_model_gradient_boosted.hpp"

#include <memory>
#include <Eigen/Dense>
#include <iostream>

namespace price_model {


GradientBoost::GradientBoost(std::shared_ptr<DataHandler> data_handler): data_handler(data_handler) {
    SplitData split_data = data_handler->get_split_data(.75, false);
    train(split_data.X_train, split_data.Y_train);
    evaluate_model(split_data.x_test, split_data.y_test);
};

void GradientBoost::evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test) {
	Eigen::VectorXd y_pred = predict(x_test);
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

Eigen::VectorXd GradientBoost::predict(const Eigen::MatrixXd& X) {
    const int trees_num = model.regression_trees.size();
    const double learning_rate = 0.01;

    Eigen::VectorXd y_pred = Eigen::VectorXd::Constant(X.rows(), model.F_0);

    for (const auto& tree : model.regression_trees) {
		y_pred += learning_rate * tree->predict(X);
	}

    return y_pred;
}

void GradientBoost::train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y) {
    assert((y.size() != 0) && "y cannot be empty!");
    assert((y.size() == X.rows()) && "The number of rows for X and y do not match!");

    double F_0 = y.mean();
    Eigen::VectorXd predictions = Eigen::VectorXd::Constant(y.size(), F_0);
    Eigen::VectorXd residues = y - predictions;

    std::vector<std::unique_ptr<RegressionTree>> regression_trees;
    regression_trees.reserve(10);

    const auto train_config = RegressionTree::TrainConfig {
        .max_depth = 12,
        .min_dataset_size = 50
    };

    const double learning_rate = 0.01;
    const int n_rounds = 1000;

    for (int i = 0; i < n_rounds; i++) {
        auto tree = std::make_unique<RegressionTree>();
        tree->train(X, residues, train_config);
        Eigen::VectorXd tree_preds = tree->predict(X);

        predictions += learning_rate * tree_preds;
        residues = y - predictions;

        regression_trees.push_back(std::move(tree));
    }

    model = { F_0, std::move(regression_trees) };
}



}
