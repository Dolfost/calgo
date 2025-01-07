#include <calgo/in/interpolation.hpp>

#include <cmath>

namespace ca::in {

ListT Nodes::uniform(double a, double b, std::size_t n) {
	double step = std::abs(a-b)/(n-1);
	ListT points;
	for (std::size_t k = 0; k < n; k++)
		points.push_back(a + k*step);

	return points;
}

ListT Nodes::chebyshev(double a, double b, std::size_t n) {
	double c1 = (a+b)/2.0;
	double c2 = (b-a)/2.0;
	ListT points;
	for (long k = n-1; k >= 0; k--)
		points.push_back(c1 + c2*std::cos(((2.0*k+1.0)/(2.0*n))*mconst.pi));

	return points;
}

}
