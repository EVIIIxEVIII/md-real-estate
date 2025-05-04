#pragma once

#include <Eigen/Dense>
#include <csv.h>

namespace price_model {

struct SplitData {
    Eigen::MatrixXd X_train;
    Eigen::VectorXd Y_train;

    Eigen::MatrixXd x_test;
    Eigen::VectorXd y_test;
};

template<std::size_t F>
struct LoadedData {
    std::vector<std::array<double, F>> X_rows;
    std::vector<double>  y_vals;
};

class DataHandler {
    public:
        DataHandler(std::string data_path) {
            load_data(data_path);
        };

        static constexpr std::array feature {
            "total_area","number_of_rooms","apartment_condition","housing_fund","region","living_room","parking_spot","number_of_floors","bathroom","floor","is_penthouse","compartmentalization"
        };

        static constexpr std::size_t F = feature.size();

        SplitData get_split_data(float training_ratio, bool add_intercept);

    private:
        LoadedData<F> loaded_data;
        void load_data(std::string data_path);
        template<std::size_t... Is>
        bool read_row_array(io::CSVReader<F + 1>& in,
                            std::array<double, F>&  x,
                            double&                 y,
                            std::index_sequence<Is...>);
};

}
