//#define RAYGUI_IMPLEMENTATION
//
//#include <memory>
//#include <cassert>
//#include <csv.h>
//#include <Eigen/Dense>
//#include <cmath>
//#include <raylib.h>
//#include "./include/raygui.h"
//
//#include "./price_model/data_handler.hpp"
//#include "./price_model/price_model_gradient_boosted.hpp"
//
//
//int main()
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
    Font font = LoadFont("resources/DejaVuSans.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    GuiSetFont(font);

    std::vector<std::unique_ptr<ui::SearchSelect>> selects;

    std::vector<std::string> living_room({ "Apartament cu living", "Apartament fara living" });
    std::vector<std::string> parking({ "Deschis", "Garaj", "Sub acoperis", "Subterana" });
    std::vector<std::string> compartmentalization({ "102", "135", "143", "Brj (Brejnevka)", "Camin familial", "Cs (Ceska)", "Garsoniera", "Hrusiovka", "Individuala", "Ms (serie moldoveneasca)", "Pe pamant", "Rubaska", "St (Stalinka)", "Vart (Varnitkaia)", "Городской", "Итальянский Двор", "Московский" });
    std::vector<std::string> condition({ "Are nevoie de reparatie", "Casa de demolat", "Constructie nefinisata", "Dat in exploatare", "Design individual", "Euroreparatie", "Fara reparatie", "Reparatie cosmetica", "Varianta alba", "Varianta sura" });
    std::vector<std::string> housing_fund({ "Constructii noi", "Secundar" });
    std::vector<std::string> regions({ "Anenii Noi (Centru)", "Bacioi (Centru)", "Bacioi (Periferie)", "Basarabeasca (Centru)", "Bender/Tighina (Balca)", "Bender/Tighina (Bam)", "Bender/Tighina (Centru)", "Bender/Tighina (Microraionul Lenin)", "Bender/Tighina (Microraionul de Nord)", "Bender/Tighina (Microraionul Selkovai)", "Biruinta (Centru)", "Briceni (Centru)", "Briceni (Periferie)", "Bubuieci (Centru)", "Bubuieci (Periferie)", "Budesti (Centru)", "Bugeac (Centru)", "Bugeac (Periferie)", "Bulboaca (Centru)", "Balti (10 cartier)", "Balti (5 cartier)", "Balti (6 cartier)", "Balti (7 cartier)", "Balti (8 cartier)", "Balti (9 cartier)", "Balti (Autogara)", "Balti (BAM)", "Balti (Centru)", "Balti (Dvoreanskoe gnezdo)", "Balti (Gara de nord)", "Balti (Molodova)", "Balti (Paminteni)", "Balti (Podul Chisinaului)", "Balti (Pole ciudes)", "Balti (Steluta)", "Cahul (Centru)", "Cahul (Periferie)", "Ceadir-Lunga (Centru)", "Ceadir-Lunga (Periferie)", "Chetrosu (Centru)", "Chisinau (Aeroport)", "Chisinau (Botanica)", "Chisinau (Buiucani)", "Chisinau (Centru)", "Chisinau (Ciocana)", "Chisinau (Durlești)", "Chisinau (Posta Veche)", "Chisinau (Rascani)", "Chisinau (Sculeni)", "Chisinau (Telecentru)", "Cimislia (Centru)", "Ciorescu (Centru)", "Ciorescu (Periferie)", "Codru (Centru)", "Codru (Periferie)", "Cojusna (Centru)", "Cojusna (Periferie)", "Colonita (Centru)", "Colonita (Periferie)", "Comrat (Centru)", "Comrat (Periferie)", "Congaz (Centru)", "Cornesti (Centru)", "Costesti (Centru)", "Cricova (Centru)", "Cricova (Periferie)", "Criuleni (Centru)", "Cupcini (Centru)", "Cainari (Centru)", "Calarasi (Centru)", "Calarasi (Periferie)", "Causeni (Centru)", "Causeni (Periferie)", "Dobrogea (Centru)", "Donduseni (Centru)", "Drochia (Centru)", "Drochia (Periferie)", "Dubasari (Centru)", "Dumbrava (Centru)", "Dumbrava (Periferie)", "Danceni (Centru)", "Edinet (Centru)", "Edinet (Periferie)", "Elizaveta (Periferie)", "Floreni (Centru)", "Floreni (Periferie)", "Floresti (Centru)", "Floresti (Periferie)", "Falesti (Centru)", "Falesti (Periferie)", "Ghidighici (Centru)", "Glodeni (Centru)", "Glodeni (Periferie)", "Grigoriopol (Centru)", "Gratiesti (Centru)", "Galesti (Centru)", "Hincesti (Centru)", "Hincesti (Periferie)", "Hirjauca (Centru)", "Ialoveni (Centru)", "Ialoveni (Periferie)", "Leova (Centru)", "Leova (Periferie)", "Merenii Noi (Centru)", "Micauti (Centru)", "Nisporeni (Centru)", "Ocnita (Centru)", "Ocnita (Periferie)", "Orhei (Centru)", "Orhei (Periferie)", "Otaci (Centru)", "Pelivan (Centru)", "Peresecina (Centru)", "Piatra Alba (Centru)", "Piatra Alba (Periferie)", "Porumbeni (Centru)", "Rezina (Centru)", "Ribnita (Centru)", "Riscani (Centru)", "Riscani (Periferie)", "Soroca (Centru)", "Soroca (Periferie)", "Speia (Centru)", "Stejareni (Centru)", "Straseni (Centru)", "Straseni (Periferie)", "Stauceni (Centru)", "Svetlii (Centru)", "Singera (Centru)", "Singera (Periferie)", "Singerei (Centru)", "Singerei (Periferie)", "Taraclia (Centru)", "Taraclia (Periferie)", "Telenesti (Centru)", "Tiraspol (Borodinka)", "Tiraspol (Centru)", "Tiraspol (Fedico)", "Tiraspol (Mecinikov)", "Tiraspol (Microraionul de Vest)", "Tohatin (Centru)", "Truseni (Centru)", "Truseni (Periferie)", "Ungheni (Beresti)", "Ungheni (Centru)", "Ungheni (Danuteni)", "Ungheni (Tineret)", "Vadul lui Voda (Periferie)", "Varnita (Centru)", "Vatra (Centru)", "Vatra (Periferie)", "Visniovca (Centru)", "Vulcanesti (Centru)", "Soldanesti (Centru)", "Tantareni (Centru)" });

    selects.emplace_back(std::make_unique<ui::SearchSelect>(living_room));
    selects.emplace_back(std::make_unique<ui::SearchSelect>(parking));
    selects.emplace_back(std::make_unique<ui::SearchSelect>(compartmentalization));
    selects.emplace_back(std::make_unique<ui::SearchSelect>(condition));
    selects.emplace_back(std::make_unique<ui::SearchSelect>(housing_fund));
    selects.emplace_back(std::make_unique<ui::SearchSelect>(regions));

	while (!WindowShouldClose()) {
        for (auto& select: selects) {
            select->before_draw();
        }

		BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            for (size_t i = 0; i < selects.size(); i++) {
                selects[i]->draw(i, 20);
            }

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
