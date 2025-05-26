#include<iostream>

#include<calgo/num/boundary_value_problem.hpp>
#include<calgo/num/simpson.hpp>
#include<calgo/in/interpolation.hpp>

int main() {
	std::size_t n = 100;
	ca::num::bvp_galerkin<double> ec;
	ca::in::UniformNodes<double> nodes(0, ca::mconst.pi/4, n);

	ec.set_f([](auto x) { return 0; });
	ec.set_q([](auto x) { return -4; });
	ec.set_k([](auto x) { return 1; });
	ec.set_nodes(&nodes);
	ec.set_mu({-2, 10});

	ca::num::bvp_galerkin<double>::integral_estimator est = 
		[&nodes](
			const ca::num::bvp_galerkin<double>::integral_function& f, 
			double a,
			double b
		) {
			ca::num::simpson<double> s;
			ca::in::UniformNodes<double> t(a, b, 200);
			s.set_nodes(&t);
			s.set_f(f);
			return s.integrate_safe();
	};
	ec.set_int_estimator(est);

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
