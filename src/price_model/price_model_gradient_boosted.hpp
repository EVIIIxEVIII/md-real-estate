#include <memory>
#include <stdint.h>
#include "data_handler.hpp"

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


    private:
        std::shared_ptr<DataHandler> data_handler;
        void train(Eigen::MatrixXd X, Eigen::VectorXd y);
};


}
