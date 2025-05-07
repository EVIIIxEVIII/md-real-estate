#pragma once

#include <vector>
#include <memory>

#include "xgboost_tree.hpp"

namespace price_model {

class XGBoost {

    public:
        XGBoost(XGBoostConfig config): config(config) {};
        Eigen::VectorXd predict(const Eigen::MatrixXd& X);
        void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);

    private:
        struct Model {
            double initial = 0.5;
            std::vector<std::unique_ptr<XGBoostTree>> xgboost_trees;
        };

        XGBoostConfig config;
        Model model;
};

}
