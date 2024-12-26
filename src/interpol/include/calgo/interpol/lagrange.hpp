#ifndef _CALGO_INTERPOL_LAGRANGE_
#define _CALGO_INTERPOL_LAGRANGE_

#include <calgo/interpol/interpol.hpp>

namespace ca::interpol {

class Lagrange: public ExplicitInterpolation {
public:
	using ExplicitInterpolation::Interpolation;

	virtual void setX(ListT x) override;

	virtual double interpolate(double x) override;
protected:
	ListT n_L;
	void calculateL();
};

}

#endif // !_CALGO_INTERPOL_LAGRANGE_
