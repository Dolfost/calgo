#ifndef _CALGO_INTERPOL_NEWTON_BACKWARD_
#define _CALGO_INTERPOL_NEWTON_BACKWARD_

#include <calgo/interpol/interpol.hpp>

namespace ca::interpol {

class NewtonBackward: public ExplicitInterpolation {
public:
	using ExplicitInterpolation::Interpolation;

	virtual void setX(ListT x) override;

	virtual double interpolate(double x) override;
	const ListT& dividedDifference = n_dif;
protected:
	ListT n_dif;
	void calculateDiff();
};

}

#endif // !_CALGO_INTERPOL_NEWTON_BACKWARD_
