#ifndef _CALGO_IN_LAGRANGE_
#define _CALGO_IN_LAGRANGE_

#include <calgo/in/interpolation.hpp>

namespace ca::in {

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

#endif // !_CALGO_IN_LAGRANGE_
