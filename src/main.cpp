//#include <memory>
//#include <cassert>
//#include <csv.h>
//#include <Eigen/Dense>
//#include <cmath>
//#include "./price_model/price_model_gradient_boosted.hpp"
//
//int main()
//{
//    auto data_handler = std::make_shared<price_model::DataHandler>("./data/datasets/non_encoded_df.csv");
//    price_model::SplitData split_data = data_handler->get_split_data(.75, false);
//
//    price_model::GradientBoostConfig gradient_boost_config{
//        .n_estimators = 2000,
//        .learning_rate = 0.005,
//        .max_depth = 12,
//        .min_dataset_size = 50,
//    };
//
//    auto grad_boosted_price_model = std::make_unique<price_model::GradientBoost>(gradient_boost_config);
//    grad_boosted_price_model->train(split_data.X_train, split_data.Y_train);
//    grad_boosted_price_model->evaluate_model(split_data.x_test, split_data.y_test);
//}

#include <memory>
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include "ui/search_select.hpp"

#include <raylib.h>
#include <string>
#include <vector>

int main(void)
{
	InitWindow(420, 240, "raygui — select with search");
	SetTargetFPS(60);

    std::vector<std::string> options({
        "Apple", "Apricot", "Avocado",
        "Banana", "Blackberry",
        "Cherry", "Coconut", "Cranberry"
    });

    auto search_select = std::make_unique<ui::SearchSelect>(options);

	while (!WindowShouldClose()) {
        search_select->before_draw();

		BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            search_select->draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
