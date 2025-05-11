#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

#include "price_model/data_handler.hpp"
#include "price_model/price_model_xgboost.hpp"
#include "ui/estimation_view.hpp"
#include "ui/mortgage_view.hpp"

#include <memory>
#include <raylib.h>
#include <string>
#include <charconv>

#define WIDTH 1920
#define HEIGHT 1080

float center(float width) {
    return (WIDTH - width) / 2.;
}

double to_double(const char* buf)
{
    double v = 0;
    auto res = std::from_chars(buf, buf + std::strlen(buf), v);
    if (res.ec == std::errc{}) return v;
    return std::strtod(buf, nullptr);
}

std::unique_ptr<price_model::XGBoost> train_model() {
    auto data_handler = std::make_shared<price_model::DataHandler>("./data/datasets/non_encoded_df.csv");
    price_model::SplitData split_data = data_handler->get_split_data(.90);

    price_model::XGBoostConfig xgboost_config{
        .n_estimators = 1000,
        .eta = 0.01,
        .tree_config = price_model::XGBoostConfig::XGBoostTreeConfig {
            .lambda = 5.0,
            .gamma = 0.0,
            .max_depth = 6
        },
    };

    auto grad_boosted_price_model = std::make_unique<price_model::XGBoost>("./models/xgboost_model.bin");
    grad_boosted_price_model->train(split_data.X_train, split_data.Y_train);
    grad_boosted_price_model->evaluate_model(split_data.x_test, split_data.y_test);

    grad_boosted_price_model->save_model("./models/xgboost_model.bin");

    return grad_boosted_price_model;
}

int main(void)
{
    auto estimation_view    = std::make_unique<ui::EstimationView>( WIDTH);
    auto mortgage_view		= std::make_unique<ui::MortgageView>(WIDTH);

	InitWindow(WIDTH, HEIGHT, "raygui — select with search");
	SetTargetFPS(60);
    Font font = LoadFontEx("resources/DejaVuSans.ttf", 32, nullptr, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    GuiSetFont(font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    double fair_price = 0.0;
    bool in_mortgage = false;

	while (!WindowShouldClose()) {
        if(!in_mortgage)	estimation_view->before_draw();
        else				mortgage_view->before_draw();

		BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if(!in_mortgage) {
				estimation_view->draw();

				if(estimation_view->get_prediction()!=0 && GuiButton({ center(280)+150, 650, 280, 24 }, "Mortgage analysis")) {
					mortgage_view->set_fair_price(estimation_view->get_prediction());
					estimation_view->hide();
					in_mortgage = true;
				}
			}  else {
				mortgage_view->draw();

				if(GuiButton({ center(120), 820, 120, 24 }, "Back")) {
					estimation_view->show();
					in_mortgage = false;
				}
			}


		EndDrawing();
	}

	CloseWindow();
	return 0;
}
