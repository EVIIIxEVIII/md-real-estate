#include "mortgage_view.hpp"
#include "../include/raygui.h"
#include <charconv>
#include <cmath>
#include <raylib.h>

namespace ui {

MortgageView::MortgageView(float screen_width) : _screen_width(screen_width)
{
	_inputs.emplace_back(std::make_unique<ui::Input>("Offer price ($)",			250, _offer_price));
	_inputs.emplace_back(std::make_unique<ui::Input>("Down-payment (%)",		250, _down_payment));
	_inputs.emplace_back(std::make_unique<ui::Input>("Interest rate (APR%)",	250, _interest_rate));
	_inputs.emplace_back(std::make_unique<ui::Input>("Term (years)",			250, _term_years));
	_inputs.emplace_back(std::make_unique<ui::Input>("Property tax ($/yr)",		250, _prop_tax));
	_inputs.emplace_back(std::make_unique<ui::Input>("Insurance ($/yr)",		250, _insurance));
	_inputs.emplace_back(std::make_unique<ui::Input>("HOA / maint. ($/mo)",		250, _hoa));
	_inputs.emplace_back(std::make_unique<ui::Input>("Monthly income ($)",		250, _income));
	_inputs.emplace_back(std::make_unique<ui::Input>("Other debt ($/mo)",		250, _other_debt));
}

double MortgageView::to_double(const char *buf)
{
	double v = 0.0;
	auto r = std::from_chars(buf, buf + std::strlen(buf), v);
	if(r.ec == std::errc{})	return v;
	return std::strtod(buf, nullptr);
}

void MortgageView::before_draw()
{
	if(!_visible) return;
}

void MortgageView::draw()
{
	if(!_visible) return;

	const float first_row_y = 200.f;
	const float col_spacing = 270.f;

	for(size_t i = 0; i < _inputs.size(); ++i)
	{
		size_t row = i / 3;
		size_t col = i % 3;
		float x_off = col * col_spacing + center(3.f * col_spacing);
		float y_off = first_row_y + row * 80.f;
		_inputs[i]->draw(0, y_off, x_off);
	}


	if(GuiButton({ center(280.f), 550.f, 280, 32 }, "Calculate"))
	{
        auto calc = [&](double price)->Metrics {
            Metrics m;
            double dp_pct = to_double(_down_payment) / 100.0;
            double loan    = price * (1.0 - dp_pct);
            double rate    = std::max(0.0001, to_double(_interest_rate) / 100.0);
            double term_y  = std::max(1.0,   to_double(_term_years));
            double r_m     = rate / 12.0;
            int    N       = static_cast<int>(term_y * 12);

            m.pmt = loan * r_m * std::pow(1+r_m, N) / (std::pow(1+r_m, N) - 1);
            m.total = m.pmt * N;
            double piti = m.pmt +
                          to_double(_prop_tax)/12.0 +
                          to_double(_insurance)/12.0 +
                          to_double(_hoa);
            m.piti  = piti;
            double inc = to_double(_income);
            if(inc>0) {
                m.front =  piti / inc;
                m.back  = (piti + to_double(_other_debt)) / inc;
            }
            m.ltv = _fair_price>0 ? loan / _fair_price : 0.0;
            return m;
        };

        _fair  = (_fair_price > 0)         ? calc(_fair_price) : Metrics{};
        _offer = (std::strlen(_offer_price)) ? calc(to_double(_offer_price)) : Metrics{};
    }

    auto pick_colour = [](const std::string& key, double v)->Color
    {
        const Color OK  = GREEN;
        const Color MID = ORANGE;
        const Color BAD = RED;

        if(key=="FrontDTI")
            return (v<=25.0)?OK : (v<=28.0?MID:BAD);

        if(key=="BackDTI")
            return (v<=33.0)?OK : (v<=36.0?MID:BAD);

        if(key=="LTV")
            return (v<=80.0)?OK : (v<=90.0?MID:BAD);

        if(key=="PITIratio")
            return (v<=30.0)?OK : (v<=40.0?MID:BAD);

        if(key=="Delta%")
            return (std::fabs(v)<=10.0) ? OK : (std::fabs(v) <= 15.0 ? MID:BAD);

        return LIGHTGRAY;
    };

    const float table_w   = 640.f;
	const float col_w[3]  = { 240.f, 190.f, 190.f };
	const float row_h     = 24.f;
	const float x0        = center(table_w);
	float y0              = 610.f;

	GuiGroupBox({ x0, y0, table_w, 8*row_h + 40 }, "Results");
	y0 += 20;

    auto row = [&](const char *label,
                   double fair_v, double off_v,
                   int r, const std::string& key = "")
    {
        GuiLabel({ x0+10, y0+r*row_h, col_w[0]-20, row_h }, TextFormat("%s", label));
        GuiLabel({ x0+col_w[0], y0+r*row_h, col_w[1], row_h }, TextFormat("%.2f", fair_v));

        int prev = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);
        Color c  = key.empty()?LIGHTGRAY:pick_colour(key, off_v);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(c));

        GuiLabel({ x0+col_w[0]+col_w[1],  y0+r*row_h, col_w[2],  row_h }, TextFormat("%.2f", off_v));
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, prev);
    };

	GuiLabel({ x0+col_w[0],          y0-20, col_w[1], row_h }, "Fair-Price");
	GuiLabel({ x0+col_w[0]+col_w[1], y0-20, col_w[2], row_h }, "Offer");

    row("Monthly P&I ($)" , _fair.pmt,  _offer.pmt, 0);
    row("Total P&I ($)" , _fair.total,  _offer.total, 1);

    double piti_ratio = (_offer.piti>0 && to_double(_income)>0) ? _offer.piti / to_double(_income) * 100.0 : 0.0;

    row("PITI          ($)", _fair.piti, _offer.piti, 2, "PITI");
    row("PITI ratio    (%)", _fair.piti, _offer.piti * 100. / to_double(_income), 3, "PITIratio");
    row("Front-end DTI (%)", _fair.front*100, _offer.front*100, 4, "FrontDTI");
    row("Back-end DTI  (%)", _fair.back*100,  _offer.back*100 , 5, "BackDTI");
    row("LTV (%)"          , _fair.ltv*100,   _offer.ltv*100  , 6, "LTV");

    double delta_pct = (_fair_price>0)? (to_double(_offer_price)-_fair_price)/_fair_price*100.0 : 0.0;
    row("Offer - Fair (%)" ,0.0, (to_double(_offer_price) - _fair_price) * 100. / _fair_price, 7,"Delta%");
}

}

