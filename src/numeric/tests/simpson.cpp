#include <calgo/num/simpson.hpp>

#include <iostream>
#include <cmath>

int main(int argc, char** argv) {
	ca::num::simpson<double> s;
	s.set_f([](auto x) { return std::sin(x) - std::cos(2*x) + 2; });
	// sinx-cos2x+2
	auto nodes = std::unique_ptr<ca::in::UniformNodes<double>>(new ca::in::UniformNodes<double>(-1, 6, 8));
	s.set_nodes(nodes.get());

	double i = s.integrate_safe();
	double ie = 13.3937;
	std::cout << "Integral: " << i << std::endl;
	std::cout << "Expected: " << ie << std::endl;

	return std::abs(i - ie) > 0.8;
}
