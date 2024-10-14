#ifndef _CALGO_CUBIC_INTERPOL_HPP_
#define _CALGO_CUBIC_INTERPOL_HPP_

#include <interpol/interpol.hpp>

namespace ca::interpol {

class Cubic: public Interpolation {
public:
	using Interpolation::Interpolation;

	virtual void setX(ListT x) override;

	virtual double interpolate(double x) override;
	const ListT& dividedDifference = n_M;
protected:
	ListT n_M;
	void calculateM();
};

}

#endif // !_CALGO_CUBIC_INTERPOL_HPP_
