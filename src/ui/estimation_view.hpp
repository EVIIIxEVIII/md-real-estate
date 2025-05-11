#pragma once
#include <vector>
#include <memory>
#include <Eigen/Dense>

#include "input.hpp"
#include "search_select.hpp"
#include "../price_model/price_model_xgboost.hpp"

namespace ui {

class EstimationView {
    public:
        void before_draw();
        void draw();

        void hide();
        void show();
        inline double get_prediction() {return _prediction(0);}

        EstimationView(float screen_width);

        static double to_double(const char* buf);
        static float center(float width, float screen_width);

    private:
        char _total_area[256]       = {};
        char _number_of_rooms[256]  = {};
        char _number_of_floors[256] = {};
        char _bathrooms[256]        = {};
        char _floor[256]            = {};

        int _living_room_val          = -1;
        int _parking_val              = -1;
        int _compartmentalization_val = -1;
        int _condition_val            = -1;
        int _housing_fund_val         = -1;
        int _region_val               = -1;
        int _is_penthouse_val         = -1;

        std::vector<std::unique_ptr<ui::Input>>        _features_inputs;
        std::vector<std::unique_ptr<ui::SearchSelect>> _features_selects;

        Eigen::VectorXd _prediction;
        float           _screen_width;
        std::unique_ptr<price_model::XGBoost> _model;

        bool _display = true;
};


}
