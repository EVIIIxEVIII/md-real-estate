#include <memory>
#include <cassert>
#include <csv.h>
#include <Eigen/Dense>
#include <cmath>
#include "./price_model/price_model_ols.hpp"

int main()
{
    auto data_handler = std::make_shared<price_model::DataHandler>("./data/datasets/encoded_df.csv");
    auto price_model = std::make_unique<price_model::Ols>(data_handler);
}
