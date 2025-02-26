#include<iostream>
#include<cmath>

#include<calgo/num/runge_kutta.hpp>
#include<calgo/in/interpolation.hpp>

int main() {
	std::size_t n = 100;
	ca::num::runge_kutta<double> ec;
	ca::in::ChebyshevNodes<double> nodes(0, ca::mconst.pi/2, n);
	ec.set_f([](auto t, auto u) { return u*std::sin(t); });
	ec.set_nodes(&nodes);
	ec.set_u0(1);
	ec.solve_safe();
	std::cout << "Paste the output to desmos.com/calculator\n\n";

	double h = (nodes.b() - nodes.a())/(n-1);
	std::cout << "X = [";
	for (std::size_t i = 0; i < n; i++)
		std::cout << nodes.a() + i*h << ", ";
	std::cout << "\b\b]\nY = [";

	for (std::size_t i = 0; i < n; i++)
		std::cout << ec.y()[i] << ", ";
	std::cout << "\b\b]\n(X, Y)\ny=e^{1-cos(x)}\n";

	return 0;
}
