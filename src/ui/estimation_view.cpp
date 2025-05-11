#include "estimation_view.hpp"
#include "../include/raygui.h"
#include "../price_model/price_model_xgboost.hpp"

#include <charconv>

#include <Eigen/Dense>

namespace ui {

void EstimationView::show() { _display = true; }
void EstimationView::hide() { _display = false; }

EstimationView::EstimationView(float screen_width) {
    _prediction.resize(1);
    _prediction(0) = 0.0;
    _model = std::make_unique<price_model::XGBoost>("./models/xgboost_model.bin");
    _screen_width = screen_width;

    _features_inputs.emplace_back(std::make_unique<ui::Input>("Total area", 250, _total_area));
    _features_inputs.emplace_back(std::make_unique<ui::Input>("Number of rooms", 250, _number_of_rooms));
    _features_inputs.emplace_back(std::make_unique<ui::Input>("Number of floors", 250, _number_of_floors));
    _features_inputs.emplace_back(std::make_unique<ui::Input>("Bathrooms", 250, _bathrooms));
    _features_inputs.emplace_back(std::make_unique<ui::Input>("Floor", 250, _floor));

    std::vector<std::string> is_penthouse({ "No", "Yes" });
    std::vector<std::string> living_room({ "With living room", "Without living room" });
    std::vector<std::string> parking({ "Open", "Garage", "Covered parking", "Underground" });
    std::vector<std::string> compartmentalization({ "102", "135", "143", "Brj (Brejnevka)", "Camin familial", "Cs (Ceska)", "Garsoniera", "Hrusiovka", "Individuala", "Ms (serie moldoveneasca)", "Pe pamant", "Rubaska", "St (Stalinka)", "Vart (Varnitkaia)", "Gorodskoi", "Italianskii Dvor", "Moskovschii" });
    std::vector<std::string> condition({ "Needs renovation", "House to be demolished", "Unfinished construction", "Commissioned", "Custom design", "European-style renovation", "No renovation", "Cosmetic renovation", "White box", "Gray box" });

    std::vector<std::string> housing_fund({ "New building", "Old building" });
    std::vector<std::string> regions({ "Anenii Noi (Centru)", "Bacioi (Centru)", "Bacioi (Periferie)", "Basarabeasca (Centru)", "Bender/Tighina (Balca)", "Bender/Tighina (Bam)", "Bender/Tighina (Centru)", "Bender/Tighina (Microraionul Lenin)", "Bender/Tighina (Microraionul de Nord)", "Bender/Tighina (Microraionul Selkovai)", "Biruinta (Centru)", "Briceni (Centru)", "Briceni (Periferie)", "Bubuieci (Centru)", "Bubuieci (Periferie)", "Budesti (Centru)", "Bugeac (Centru)", "Bugeac (Periferie)", "Bulboaca (Centru)", "Balti (10 cartier)", "Balti (5 cartier)", "Balti (6 cartier)", "Balti (7 cartier)", "Balti (8 cartier)", "Balti (9 cartier)", "Balti (Autogara)", "Balti (BAM)", "Balti (Centru)", "Balti (Dvoreanskoe gnezdo)", "Balti (Gara de nord)", "Balti (Molodova)", "Balti (Paminteni)", "Balti (Podul Chisinaului)", "Balti (Pole ciudes)", "Balti (Steluta)", "Cahul (Centru)", "Cahul (Periferie)", "Ceadir-Lunga (Centru)", "Ceadir-Lunga (Periferie)", "Chetrosu (Centru)", "Chisinau (Aeroport)", "Chisinau (Botanica)", "Chisinau (Buiucani)", "Chisinau (Centru)", "Chisinau (Ciocana)", "Chisinau (Durlești)", "Chisinau (Posta Veche)", "Chisinau (Rascani)", "Chisinau (Sculeni)", "Chisinau (Telecentru)", "Cimislia (Centru)", "Ciorescu (Centru)", "Ciorescu (Periferie)", "Codru (Centru)", "Codru (Periferie)", "Cojusna (Centru)", "Cojusna (Periferie)", "Colonita (Centru)", "Colonita (Periferie)", "Comrat (Centru)", "Comrat (Periferie)", "Congaz (Centru)", "Cornesti (Centru)", "Costesti (Centru)", "Cricova (Centru)", "Cricova (Periferie)", "Criuleni (Centru)", "Cupcini (Centru)", "Cainari (Centru)", "Calarasi (Centru)", "Calarasi (Periferie)", "Causeni (Centru)", "Causeni (Periferie)", "Dobrogea (Centru)", "Donduseni (Centru)", "Drochia (Centru)", "Drochia (Periferie)", "Dubasari (Centru)", "Dumbrava (Centru)", "Dumbrava (Periferie)", "Danceni (Centru)", "Edinet (Centru)", "Edinet (Periferie)", "Elizaveta (Periferie)", "Floreni (Centru)", "Floreni (Periferie)", "Floresti (Centru)", "Floresti (Periferie)", "Falesti (Centru)", "Falesti (Periferie)", "Ghidighici (Centru)", "Glodeni (Centru)", "Glodeni (Periferie)", "Grigoriopol (Centru)", "Gratiesti (Centru)", "Galesti (Centru)", "Hincesti (Centru)", "Hincesti (Periferie)", "Hirjauca (Centru)", "Ialoveni (Centru)", "Ialoveni (Periferie)", "Leova (Centru)", "Leova (Periferie)", "Merenii Noi (Centru)", "Micauti (Centru)", "Nisporeni (Centru)", "Ocnita (Centru)", "Ocnita (Periferie)", "Orhei (Centru)", "Orhei (Periferie)", "Otaci (Centru)", "Pelivan (Centru)", "Peresecina (Centru)", "Piatra Alba (Centru)", "Piatra Alba (Periferie)", "Porumbeni (Centru)", "Rezina (Centru)", "Ribnita (Centru)", "Riscani (Centru)", "Riscani (Periferie)", "Soroca (Centru)", "Soroca (Periferie)", "Speia (Centru)", "Stejareni (Centru)", "Straseni (Centru)", "Straseni (Periferie)", "Stauceni (Centru)", "Svetlii (Centru)", "Singera (Centru)", "Singera (Periferie)", "Singerei (Centru)", "Singerei (Periferie)", "Taraclia (Centru)", "Taraclia (Periferie)", "Telenesti (Centru)", "Tiraspol (Borodinka)", "Tiraspol (Centru)", "Tiraspol (Fedico)", "Tiraspol (Mecinikov)", "Tiraspol (Microraionul de Vest)", "Tohatin (Centru)", "Truseni (Centru)", "Truseni (Periferie)", "Ungheni (Beresti)", "Ungheni (Centru)", "Ungheni (Danuteni)", "Ungheni (Tineret)", "Vadul lui Voda (Periferie)", "Varnita (Centru)", "Vatra (Centru)", "Vatra (Periferie)", "Visniovca (Centru)", "Vulcanesti (Centru)", "Soldanesti (Centru)", "Tantareni (Centru)" });

    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(is_penthouse, "Is penthouse", &_is_penthouse_val));
    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(living_room, "Living room", &_living_room_val));
    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(parking, "Parking option", &_parking_val));
    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(compartmentalization, "Compartmentalization", &_compartmentalization_val));
    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(condition, "Condition", &_condition_val));
    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(housing_fund, "Housing fund", &_housing_fund_val));
    _features_selects.emplace_back(std::make_unique<ui::SearchSelect>(regions, "Region", &_region_val));

}

float EstimationView::center(float width, float screen_width) {
    return (screen_width - width) / 2.;
}

double EstimationView::to_double(const char* buf)
{
    double v = 0;
    auto res = std::from_chars(buf, buf + std::strlen(buf), v);
    if (res.ec == std::errc{}) return v;
    return std::strtod(buf, nullptr);
}

void EstimationView::before_draw() {
    if(!_display) return;
    for (auto& select: _features_selects) {
        select->before_draw();
    }
}

void EstimationView::draw() {
    if(!_display) return;
    for (size_t i = 0; i < _features_inputs.size(); i++) {
        _features_inputs[i]->draw(i, 300, center(_features_inputs.size() * 270, _screen_width));
    }

    for (size_t i = 0; i < _features_selects.size(); i++) {
        _features_selects[i]->draw(i, 400, center(_features_selects.size() * 270, _screen_width));
    }

    if (GuiButton({ center(280, _screen_width) - 150, 650, 280, 24 }, "Estimate price")) {
        Eigen::MatrixXd x(1, 12);

        x <<  to_double(_total_area),
          to_double(_number_of_rooms) + 1,
          to_double(_number_of_floors),
          to_double(_bathrooms),
          to_double(_floor),

          static_cast<double>(_condition_val),
          static_cast<double>(_housing_fund_val),
          static_cast<double>(_region_val),
          static_cast<double>(_living_room_val),
          static_cast<double>(_parking_val),
          static_cast<double>(_is_penthouse_val),
          static_cast<double>(_compartmentalization_val);

        _prediction = _model->predict(x);
    }

    GuiLabel({ center(300, _screen_width), 750, 300, 24 }, TextFormat("Estimated fair price: %.2f", _prediction(0)));
}


}
