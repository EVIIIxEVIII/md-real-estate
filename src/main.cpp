#define RAYGUI_IMPLEMENTATION

#include <memory>
#include <cassert>
#include <csv.h>
#include <Eigen/Dense>
#include <cmath>
#include <raylib.h>
#include <fstream>
#include "include/raygui.h"

#include "./price_model/data_handler.hpp"
#include "./price_model/price_model_xgboost.hpp"

void save_vector_to_csv(const Eigen::VectorXd& vec, const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) return;

	file << "Prediction" << "\n";
	for (int i = 0; i < vec.size(); ++i) {
		file << vec[i] << "\n";
	}

	file.close();
}

int main()
{
    auto data_handler = std::make_shared<price_model::DataHandler>("./data/kaggle-test/train_filtered.csv");
    //auto data_handler = std::make_shared<price_model::DataHandler>("./data/datasets/non_encoded_df.csv");
    price_model::SplitData split_data = data_handler->get_split_data(.75);

    price_model::XGBoostConfig gradient_boost_config{
        .n_estimators = 300,
        .eta = 0.3,
        .tree_config = price_model::XGBoostConfig::XGBoostTreeConfig {
            .lambda = 5.0,
            .gamma = 0.0,
            .max_depth = 6
        },
    };

    auto xgboost_price_model = std::make_unique<price_model::XGBoost>(gradient_boost_config);
    xgboost_price_model->train(split_data.X_train, split_data.Y_train);
    xgboost_price_model->evaluate_model(split_data.x_test, split_data.y_test);

    //auto test_data = std::make_shared<price_model::DataHandler>("./data/kaggle-test/test_filtered.csv");
    //auto test_data_mat = test_data->get_split_data(1.).X_train;
    //Eigen::VectorXd predictions = xgboost_price_model->predict(test_data_mat);
    //save_vector_to_csv(predictions, "./data/kaggle-test/predictions.csv");
}

//#define RAYGUI_IMPLEMENTATION
//#include "include/raygui.h"
//#include "ui/search_select.hpp"
//#include "ui/input.hpp"
//#include "price_model/data_handler.hpp"
//#include "price_model/price_model_gradient_boosted.hpp"
//
//#include <memory>
//#include <raylib.h>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <charconv>
//
//#define WIDTH 1920
//#define HEIGHT 1080
//
//float center(float width) {
//    return (WIDTH - width) / 2.;
//}
//
//double to_double(const char* buf)
//{
//    double v = 0;
//    auto res = std::from_chars(buf, buf + std::strlen(buf), v);
//    if (res.ec == std::errc{}) return v;
//    return std::strtod(buf, nullptr);
//}
//
//int main(void)
//{
//    auto data_handler = std::make_shared<price_model::DataHandler>("./data/datasets/non_encoded_df.csv");
//    price_model::SplitData split_data = data_handler->get_split_data(.85);
//
//    price_model::GradientBoostConfig gradient_boost_config{
//        .n_estimators = 1000,
//        .learning_rate = 0.01,
//        .max_depth = 12,
//        .min_dataset_size = 50,
//    };
//
//    auto grad_boosted_price_model = std::make_unique<price_model::GradientBoost>(gradient_boost_config);
//    grad_boosted_price_model->train(split_data.X_train, split_data.Y_train);
//    grad_boosted_price_model->evaluate_model(split_data.x_test, split_data.y_test);
//
//    Eigen::MatrixXd test_X = split_data.x_test.row(0);
//    Eigen::VectorXd test_prediction = grad_boosted_price_model->predict(test_X);
//
//    std::cout << "Values: " << test_X << "\n";
//    std::cout << "Prediction: " << test_prediction << "\n";
//
//	InitWindow(WIDTH, HEIGHT, "raygui — select with search");
//	SetTargetFPS(60);
//    Font font = LoadFontEx("resources/DejaVuSans.ttf", 32, nullptr, 0);
//    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
//    GuiSetFont(font);
//    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
//
//    char total_area[256] = {};
//    char number_of_rooms[256] = {};
//    char number_of_floors[256] = {};
//    char bathrooms[256] = {};
//    char floor[256] = {};
//
//    int living_room_val = -1;
//    int parking_val = -1;
//    int compartmentalization_val = -1;
//    int condition_val = -1;
//    int housing_fund_val = -1;
//    int region_val = -1;
//    int is_penthouse_val = -1;
//
//    //"total_area","number_of_rooms","number_of_floors","bathroom","floor","apartment_condition","housing_fund","region","living_room","parking_spot","is_penthouse","compartmentalization"
//
//    std::vector<std::unique_ptr<ui::Input>> inputs;
//    inputs.emplace_back(std::make_unique<ui::Input>("Total area", 250, total_area));
//    inputs.emplace_back(std::make_unique<ui::Input>("Number of rooms", 250, number_of_rooms));
//    inputs.emplace_back(std::make_unique<ui::Input>("Number of floors", 250, number_of_floors));
//    inputs.emplace_back(std::make_unique<ui::Input>("Bathrooms", 250, bathrooms));
//    inputs.emplace_back(std::make_unique<ui::Input>("Floor", 250, floor));
//
//    std::vector<std::unique_ptr<ui::SearchSelect>> selects;
//    std::vector<std::string> is_penthouse({ "No", "Yes" });
//    std::vector<std::string> living_room({ "With living room", "Without living room" });
//    std::vector<std::string> parking({ "Open", "Garage", "Covered parking", "Underground" });
//    std::vector<std::string> compartmentalization({ "102", "135", "143", "Brj (Brejnevka)", "Camin familial", "Cs (Ceska)", "Garsoniera", "Hrusiovka", "Individuala", "Ms (serie moldoveneasca)", "Pe pamant", "Rubaska", "St (Stalinka)", "Vart (Varnitkaia)", "Gorodskoi", "Italianskii Dvor", "Moskovschii" });
//    std::vector<std::string> condition({ "Needs renovation", "House to be demolished", "Unfinished construction", "Commissioned", "Custom design", "European-style renovation", "No renovation", "Cosmetic renovation", "White box", "Gray box" });
//
//    std::vector<std::string> housing_fund({ "New building", "Old building" });
//    std::vector<std::string> regions({ "Anenii Noi (Centru)", "Bacioi (Centru)", "Bacioi (Periferie)", "Basarabeasca (Centru)", "Bender/Tighina (Balca)", "Bender/Tighina (Bam)", "Bender/Tighina (Centru)", "Bender/Tighina (Microraionul Lenin)", "Bender/Tighina (Microraionul de Nord)", "Bender/Tighina (Microraionul Selkovai)", "Biruinta (Centru)", "Briceni (Centru)", "Briceni (Periferie)", "Bubuieci (Centru)", "Bubuieci (Periferie)", "Budesti (Centru)", "Bugeac (Centru)", "Bugeac (Periferie)", "Bulboaca (Centru)", "Balti (10 cartier)", "Balti (5 cartier)", "Balti (6 cartier)", "Balti (7 cartier)", "Balti (8 cartier)", "Balti (9 cartier)", "Balti (Autogara)", "Balti (BAM)", "Balti (Centru)", "Balti (Dvoreanskoe gnezdo)", "Balti (Gara de nord)", "Balti (Molodova)", "Balti (Paminteni)", "Balti (Podul Chisinaului)", "Balti (Pole ciudes)", "Balti (Steluta)", "Cahul (Centru)", "Cahul (Periferie)", "Ceadir-Lunga (Centru)", "Ceadir-Lunga (Periferie)", "Chetrosu (Centru)", "Chisinau (Aeroport)", "Chisinau (Botanica)", "Chisinau (Buiucani)", "Chisinau (Centru)", "Chisinau (Ciocana)", "Chisinau (Durlești)", "Chisinau (Posta Veche)", "Chisinau (Rascani)", "Chisinau (Sculeni)", "Chisinau (Telecentru)", "Cimislia (Centru)", "Ciorescu (Centru)", "Ciorescu (Periferie)", "Codru (Centru)", "Codru (Periferie)", "Cojusna (Centru)", "Cojusna (Periferie)", "Colonita (Centru)", "Colonita (Periferie)", "Comrat (Centru)", "Comrat (Periferie)", "Congaz (Centru)", "Cornesti (Centru)", "Costesti (Centru)", "Cricova (Centru)", "Cricova (Periferie)", "Criuleni (Centru)", "Cupcini (Centru)", "Cainari (Centru)", "Calarasi (Centru)", "Calarasi (Periferie)", "Causeni (Centru)", "Causeni (Periferie)", "Dobrogea (Centru)", "Donduseni (Centru)", "Drochia (Centru)", "Drochia (Periferie)", "Dubasari (Centru)", "Dumbrava (Centru)", "Dumbrava (Periferie)", "Danceni (Centru)", "Edinet (Centru)", "Edinet (Periferie)", "Elizaveta (Periferie)", "Floreni (Centru)", "Floreni (Periferie)", "Floresti (Centru)", "Floresti (Periferie)", "Falesti (Centru)", "Falesti (Periferie)", "Ghidighici (Centru)", "Glodeni (Centru)", "Glodeni (Periferie)", "Grigoriopol (Centru)", "Gratiesti (Centru)", "Galesti (Centru)", "Hincesti (Centru)", "Hincesti (Periferie)", "Hirjauca (Centru)", "Ialoveni (Centru)", "Ialoveni (Periferie)", "Leova (Centru)", "Leova (Periferie)", "Merenii Noi (Centru)", "Micauti (Centru)", "Nisporeni (Centru)", "Ocnita (Centru)", "Ocnita (Periferie)", "Orhei (Centru)", "Orhei (Periferie)", "Otaci (Centru)", "Pelivan (Centru)", "Peresecina (Centru)", "Piatra Alba (Centru)", "Piatra Alba (Periferie)", "Porumbeni (Centru)", "Rezina (Centru)", "Ribnita (Centru)", "Riscani (Centru)", "Riscani (Periferie)", "Soroca (Centru)", "Soroca (Periferie)", "Speia (Centru)", "Stejareni (Centru)", "Straseni (Centru)", "Straseni (Periferie)", "Stauceni (Centru)", "Svetlii (Centru)", "Singera (Centru)", "Singera (Periferie)", "Singerei (Centru)", "Singerei (Periferie)", "Taraclia (Centru)", "Taraclia (Periferie)", "Telenesti (Centru)", "Tiraspol (Borodinka)", "Tiraspol (Centru)", "Tiraspol (Fedico)", "Tiraspol (Mecinikov)", "Tiraspol (Microraionul de Vest)", "Tohatin (Centru)", "Truseni (Centru)", "Truseni (Periferie)", "Ungheni (Beresti)", "Ungheni (Centru)", "Ungheni (Danuteni)", "Ungheni (Tineret)", "Vadul lui Voda (Periferie)", "Varnita (Centru)", "Vatra (Centru)", "Vatra (Periferie)", "Visniovca (Centru)", "Vulcanesti (Centru)", "Soldanesti (Centru)", "Tantareni (Centru)" });
//
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(is_penthouse, "Is penthouse", &is_penthouse_val));
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(living_room, "Living room", &living_room_val));
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(parking, "Parking option", &parking_val));
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(compartmentalization, "Compartmentalization", &compartmentalization_val));
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(condition, "Condition", &condition_val));
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(housing_fund, "Housing fund", &housing_fund_val));
//    selects.emplace_back(std::make_unique<ui::SearchSelect>(regions, "Region", &region_val));
//    const float init_pos_inputs = center(inputs.size() * 270);
//    const float init_pos_selects = center(selects.size() * 270);
//
//    Eigen::VectorXd prediction(1); prediction(0) = 0;
//
//	while (!WindowShouldClose()) {
//        for (auto& select: selects) {
//            select->before_draw();
//        }
//
//		BeginDrawing();
//            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
//            for (size_t i = 0; i < inputs.size(); i++) {
//                inputs[i]->draw(i, 400, init_pos_inputs);
//            }
//
//            for (size_t i = 0; i < selects.size(); i++) {
//                selects[i]->draw(i, 500, init_pos_selects);
//            }
//
//            if (GuiButton({ center(280), 750, 280, 24 }, "Estimate price")) {
//                Eigen::MatrixXd x(1, 12);
//
//                x <<  to_double(total_area),
//                  to_double(number_of_rooms) + 1,
//                  to_double(number_of_floors),
//                  to_double(bathrooms),
//                  to_double(floor),
//
//                  static_cast<double>(condition_val),
//                  static_cast<double>(housing_fund_val),
//                  static_cast<double>(region_val),
//                  static_cast<double>(living_room_val),
//                  static_cast<double>(parking_val),
//                  static_cast<double>(is_penthouse_val),
//                  static_cast<double>(compartmentalization_val);
//
//                prediction = grad_boosted_price_model->predict(x);
//                std::cout << "prediction(0): " << prediction(0) << std::endl;
//            }
//
//            GuiLabel({ center(300), 850, 300, 24 }, TextFormat("Estimated price: %.2f", prediction(0)));
//		EndDrawing();
//	}
//
//	CloseWindow();
//	return 0;
//}
