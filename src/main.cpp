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
    price_model::SplitData split_data = data_handler->get_split_data(.90, false);
    // auto ols_price_model = std::make_unique<price_model::Ols>(data_handler);

    price_model::GradientBoostConfig gradient_boost_config{
        .n_estimators = 2000,
        .learning_rate = 0.005,
        .max_depth = 12,
        .min_dataset_size = 50,
    };

    auto grad_boosted_price_model = std::make_unique<price_model::GradientBoost>(gradient_boost_config);
    grad_boosted_price_model->train(split_data.X_train, split_data.Y_train);
    grad_boosted_price_model->evaluate_model(split_data.x_test, split_data.y_test);
}
