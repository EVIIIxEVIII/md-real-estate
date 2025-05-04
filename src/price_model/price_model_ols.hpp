#include <Eigen/Dense>
#include <csv.h>
#include <memory>
#include "data_handler.hpp"

namespace price_model {

class Ols {
    public:
        Ols(std::shared_ptr<DataHandler> data_handler);
        void evaluate_model(const Eigen::MatrixXd& x_test, const Eigen::VectorXd& y_test);
        Eigen::VectorXd predict(const Eigen::MatrixXd& X);

    private:
        std::shared_ptr<DataHandler> data_handler;
        Eigen::VectorXd model;
        std::array<double, DataHandler::NF> stds;
        std::array<double, DataHandler::NF> means;

        SplitData split_data(LoadedData<DataHandler::F> data);

        LoadedData<DataHandler::F> load_data(std::string data_path);

        void enlr(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, double lambda1, double lambda2);
        inline void ols(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
        void save_vector_binary(const std::string& filename);

        double soft_threshold(double z, double gamma);
};

}
