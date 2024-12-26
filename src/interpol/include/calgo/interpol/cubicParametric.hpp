#ifndef _CALGO_CUBIC_PARAMETRIC_INTERPOL_HPP_
#define _CALGO_CUBIC_PARAMETRIC_INTERPOL_HPP_

#include <calgo/interpol/interpol.hpp>

namespace ca::interpol {

class CubicParametric: public ParametricInterpolation {
public:
	using ParametricInterpolation::ParametricInterpolation;

	virtual void setX(ListT x) override;

	const ListT& mx = n_Mx;
	const ListT& my = n_My;
	double interpolateX(double t) override { return calculateS(t, n_Mx, i_x); };
	double interpolateY(double t) override { return calculateS(t, n_My, i_y); };

protected:
	ListT n_Mx, n_My;
	void calculateMs();
	double calculateS(double, ListT&, ListT&);
};

}

#endif // !_CALGO_CUBIC_PARAMETRIC_INTERPOL_HPP_
