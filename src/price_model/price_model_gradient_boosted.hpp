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

class GradientBoost {
    public:
        GradientBoost(std::shared_ptr<DataHandler> data_handler);
        Eigen::VectorXd predict(const Eigen::MatrixXd& X);

    private:
        struct Model {
            double F_0;
            std::vector<std::unique_ptr<RegressionTree>> regression_trees;
        };

        std::shared_ptr<DataHandler> data_handler;
        Model model;

        void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);
};

}
