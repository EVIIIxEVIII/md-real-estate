#pragma once
#include "input.hpp"

#include <memory>
#include <vector>
#include <cstring>

namespace ui {

class MortgageView {
public:
	explicit MortgageView(float screen_width);

	void	set_fair_price(double v)	{ _fair_price = v; }
	double	get_front_dti()	const		{ return _front_dti; }
	double	get_ltv()		const		{ return _ltv; }

	void	hide()	{ _visible = false;	}
	void	show()	{ _visible = true;	}
	bool	is_visible() const { return _visible; }

	void	before_draw();
	void	draw();

private:
    struct Metrics {
        double pmt = 0.0, piti = 0.0, front = 0.0, back = 0.0, ltv = 0.0, total = 0.0;
    };

	static	double	to_double(const char *buf);
	float			center(float width) const	{ return (_screen_width - width) / 2.f; }

	bool	_visible = true;
	float	_screen_width = 0.f;

	char	_offer_price[32]	= "";
	char	_down_payment[32]	= "";
	char	_interest_rate[32]	= "";
	char	_term_years[32]		= "";
	char	_prop_tax[32]		= "";
	char	_insurance[32]		= "";
	char	_hoa[32]			= "";
	char	_income[32]			= "";
	char	_other_debt[32]		= "";

	std::vector<std::unique_ptr<ui::Input>>	_inputs;

	double	_fair_price		= 0.0;
	double	_monthly_pmt	= 0.0;
	double	_piti			= 0.0;
	double	_front_dti		= 0.0;
	double	_back_dti		= 0.0;
	double	_ltv			= 0.0;

    Metrics _fair;
    Metrics _offer;
};

}

