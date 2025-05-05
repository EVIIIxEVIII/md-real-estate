#pragma once

#include <memory>
#include <stdint.h>
#include "data_handler.hpp"
#include "regression_tree.hpp"

namespace price_model {

struct Node {
    uint16_t fid;
    float thr;

    uint32_t left;
    uint32_t right;

    float value;
};

struct GradientBoostConfig {
    int n_estimators;
    double learning_rate;
    int max_depth;
    int min_dataset_size;
};

class GradientBoost {
    public:
        GradientBoost(GradientBoostConfig& config): config(config) {};
        Eigen::VectorXd predict(const Eigen::MatrixXd& X);
        void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);

    private:
        struct Model {
            double F_0;
            std::vector<std::unique_ptr<RegressionTree>> regression_trees;
        };

        GradientBoostConfig& config;
        Model model;
};

}
