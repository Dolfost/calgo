#ifndef _CALGO_IN_HPP_
#define _CALGO_IN_HPP_

#include <calgo/calgo.hpp>

namespace ca::in {

class Interpolation {
public:
	Interpolation(ListT x = {}, ListT y = {}) {
		setX(x), setY(y);
	}

	virtual void setX(ListT x) { i_x = x; i_n = i_x.size(); };
	virtual void setY(ListT y) { i_y = y; };

	ListT x() { return i_x; };
	ListT y() { return i_y; };
	virtual std::size_t n() { return i_n; };

	virtual ~Interpolation() {};

public:
	double hx(std::size_t i) {
		return i_x[i] - i_x[i-1];
	}
	double hy(std::size_t i) {
		return i_y[i] - i_y[i-1];
	}

protected:
	ListT i_x, i_y;
	std::size_t i_n;
};

class ExplicitInterpolation: public Interpolation {
public:
	using Interpolation::Interpolation;

	virtual double interpolate(double) = 0;
	double operator()(double x) { return interpolate(x); };
};

class ParametricInterpolation: public Interpolation {
public:
	using Interpolation::Interpolation;

	virtual double interpolateX(double t) = 0;
	virtual double interpolateY(double t) = 0;
	std::pair<double, double> operator()(double t) { return {interpolateX(t), interpolateY(t)}; };
};

class Nodes {
public:
	static ListT uniform(double, double, std::size_t);
	static ListT chebyshev(double, double, std::size_t);
};

}

#endif // !_CALGO_IN_HPP_
