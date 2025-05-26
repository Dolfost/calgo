#include<iostream>

#include<calgo/num/boundary_value_problem.hpp>
#include<calgo/in/interpolation.hpp>

int main() {
	std::size_t n = 100;
	ca::num::bvp_undefined_coefficients<double> ec;
	ca::in::UniformNodes<double> nodes(0, ca::mconst.pi/4, n);
	ec.set_f([](auto x) { return 0; });
	ec.set_q([](auto x) { return -4; });
	ec.set_k([](auto x) { return 1; });
	ec.set_nodes(&nodes);
	ec.set_mu({-2, 10});
	ec.solve_safe();
	std::cout << "Paste the output to desmos.com/calculator\n\n";

	double h = (nodes.b() - nodes.a())/(n-1);
	std::cout << "X = [";
	for (std::size_t i = 0; i < n; i++)
		std::cout << nodes.a() + i*h << ", ";
	std::cout << "\b\b]\nY = [";

	for (std::size_t i = 0; i < n; i++)
		std::cout << ec.y()[i] << ", ";
	std::cout << "\b\b]\n(X, Y)\ny=-2*cos(2x)+10*sin(2x)\n";

	return 0;
}
