#include<iostream>

#include<calgo/num/boundary_value_problem.hpp>
#include<calgo/num/simpson.hpp>
#include<calgo/in/interpolation.hpp>

int main() {
	std::size_t n = 50;
	ca::num::bvp_galerkin<double> ec;
	ca::in::UniformNodes<double> nodes(0, 1, n);

	ec.set_f([](auto x) { return 1; });
	ec.set_k([](auto x) { return 1; });
	ec.set_nodes(&nodes);

	ec.solve_safe();
	std::cout << "Paste the output to desmos.com/calculator\n\n";

	double h = (nodes.b() - nodes.a())/(n-1);
	std::cout << "X = [";
	for (std::size_t i = 0; i < n; i++)
		std::cout << nodes.a() + i*h << ", ";
	std::cout << "\b\b]\nY = [";

	for (std::size_t i = 0; i < n; i++)
		std::cout << ec.y()[i] << ", ";
	std::cout << "\b\b]\n(X, Y)\ny=-x^2/2+0.5x\n";

	return 0;
}
