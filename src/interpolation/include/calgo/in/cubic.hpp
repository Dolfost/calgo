#ifndef _CALGO_IN_CUBIC_HPP_
#define _CALGO_IN_CUBIC_HPP_

#include <calgo/in/interpolation.hpp>

namespace ca::in {

class Cubic: public ExplicitInterpolation {
public:
	using ExplicitInterpolation::Interpolation;

	virtual void setX(ListT x) override;

	virtual double interpolate(double x) override;
	const ListT& m = n_M;
protected:
	ListT n_M;
	void calculateM();
};

}

#endif // !_CALGO_IN_CUBIC_HPP_
