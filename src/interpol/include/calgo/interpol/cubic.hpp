#ifndef _CALGO_CUBIC_INTERPOL_HPP_
#define _CALGO_CUBIC_INTERPOL_HPP_

#include <calgo/interpol/interpol.hpp>

namespace ca::interpol {

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

#endif // !_CALGO_CUBIC_INTERPOL_HPP_
