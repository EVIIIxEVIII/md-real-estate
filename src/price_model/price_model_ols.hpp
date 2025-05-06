#pragma once

#include <Eigen/Dense>
#include <csv.h>

namespace price_model {

class Ols {
    public:
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);
        Eigen::VectorXd predict(const Eigen::MatrixXd& X);
        void enlr(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, double lambda1, double lambda2);
        void ols(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);

    private:
        Eigen::VectorXd model;
        double soft_threshold(double z, double gamma);
};

}
