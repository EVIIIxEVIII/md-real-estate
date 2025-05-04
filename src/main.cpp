#include <memory>
#include <cassert>
#include <csv.h>
#include <Eigen/Dense>
#include <cmath>
#include "./price_model/price_model_ols.hpp"
#include "./price_model/price_model_gradient_boosted.hpp"

int main()
{
    auto data_handler = std::make_shared<price_model::DataHandler>("./data/datasets/non_encoded_df.csv");
    auto ols_price_model = std::make_unique<price_model::Ols>(data_handler);
    auto grad_boosted_price_model = std::make_unique<price_model::GradientBoost>(data_handler);
}
