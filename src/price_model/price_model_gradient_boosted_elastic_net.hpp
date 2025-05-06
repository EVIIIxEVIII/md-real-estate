#pragma once

#include <memory>
#include <stdint.h>
#include "price_model_ols.hpp"
#include "regression_tree.hpp"

namespace price_model {

struct Node {
    uint16_t fid;
    float thr;

    uint32_t left;
    uint32_t right;

    float value;
};

struct ENGradientBoostConfig {
    int n_estimators;
    double learning_rate;
    int max_depth;
    int min_dataset_size;
};

class ENGradientBoost {
    public:
        ENGradientBoost(ENGradientBoostConfig& config): config(config) {};
        Eigen::VectorXd predict(const Eigen::MatrixXd& X);
        void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);

    private:
        struct Model {
            std::unique_ptr<Ols> F_0;
            std::vector<std::unique_ptr<RegressionTree>> regression_trees;
        };

        ENGradientBoostConfig& config;
        Model model;
};

}
