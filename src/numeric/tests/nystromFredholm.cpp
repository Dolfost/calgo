#include<iostream>
#include<cmath>

#include<calgo/num/nystromFredholm.hpp>
#include<calgo/in/interpolation.hpp>

int main() {
	ca::num::NystromFredholmSecondKind<double> nist;
	ca::in::ChebyshevNodes<double> nodes(0, ca::mconst.pi/2, 100);
	nist.setK([](auto a, auto b) { return std::sin(a)*std::cos(b); });
	nist.setF([](auto a) { return std::sin(a); });
	nist.setNodes(&nodes);
	nist.solve_safe();
	std::cout << "Paste the output to desmos.com/calculator\n\n";

	std::size_t n = 200;
	double h = (nodes.b() - nodes.a())/(n-1);
	std::cout << "X = [";
	for (std::size_t i = 0; i < n; i++)
		std::cout << nodes.a() + i*h << ", ";
	std::cout << "\b\b]\nY = [";

	for (std::size_t i = 0; i < n; i++)
		std::cout << nist(nodes.a() + i*h) << ", ";
	std::cout << "\b\b]\n(X, Y)\n";

	return 0;
}
