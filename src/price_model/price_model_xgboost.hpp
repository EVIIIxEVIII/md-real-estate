#pragma once

#include <vector>
#include <memory>

#include "xgboost_tree.hpp"

namespace price_model {

class XGBoost {
    public:
        XGBoost(XGBoostConfig config): _config(config) {};
        XGBoost(const std::string path);

        Eigen::VectorXd predict(const Eigen::MatrixXd& X);
        void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);
        void save_model(const std::string path);

    private:
        struct Model {
            double initial = 0.5;
            std::vector<std::unique_ptr<XGBoostTree>> xgboost_trees;
        };

        void load_model(const std::string path);
        XGBoostConfig _config;
        Model _model;
};

}
