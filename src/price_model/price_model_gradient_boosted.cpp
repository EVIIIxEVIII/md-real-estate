#include "price_model_gradient_boosted.hpp"
#include <memory>
#include <Eigen/Dense>


namespace price_model {


GradientBoost::GradientBoost(std::shared_ptr<DataHandler> data_handler): data_handler(data_handler) {
    SplitData split_data = data_handler->get_split_data(.75, false);



};




void GradientBoost::train(Eigen::MatrixXd X, Eigen::VectorXd y) {
    double F_0 = y.mean();
    Eigen::VectorXd r_0 = y.array() - y.mean();




}



}
