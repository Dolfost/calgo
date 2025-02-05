#ifndef _CALGO_OPT_TRANSPORTATION_SIMPLEX_TESTING_TEST_HPP_
#define _CALGO_OPT_TRANSPORTATION_SIMPLEX_TESTING_TEST_HPP_

#include <calgo/opt/transportationSimplex.hpp>

// headers required in the tests
#include <iostream>
#include <cmath>

#define CA_OPT_TRANSPORTATION_SIMPLEX_TEST(SOL, ...) \
int main(int argc, char** argv) { \
 \
	ca::vec<double> f = demand; \
	ca::vec<double> c = supply; \
	ca::mat<double> v = cost; \
	ca::opt::TransportationSimplex<double> simplex; \
	simplex.setDemand(f); \
	simplex.setSupply(c); \
	simplex.setCost(v); \
	simplex.initCallback([&simplex](auto ctx) { \
		std::cout << "\n\n--->> INITIALIZED" << std::endl \
			<< simplex << "\ndistribution:\n" << ctx.distribution \
			<< "u: " << ctx.u << "\nv: " << ctx.v << std::endl << "basis: "; \
		for (auto& x: ctx.basisCells) { \
			std::cout << '{' << x.i << ',' << x.j << "} "; \
		} \
		std::cout << "\nf: " << ctx.f << std::endl << std::endl; \
	}); \
	simplex.iterationCallback([&simplex](auto ctx, auto it) { \
		std::cout << "\n\n--->> ITERATION No " << it << std::endl \
			<< simplex << "\ndistribution:\n" << ctx.distribution \
			<< "u: " << ctx.u << "\nv: " << ctx.v << std::endl << "basis: "; \
		for (auto& x: ctx.basisCells) { \
			std::cout << '{' << x.i << ',' << x.j << "} "; \
		} \
		std::cout << "\nf: " << ctx.f << std::endl; \
	}); \
	simplex.cycleFoundCallback([&simplex](auto ctx, auto cycle, auto entering, auto leaving) { \
		std::cout << "cycle: "; \
		for (auto const& x : cycle) { \
			std::cout << x << ' '; \
		} \
		std::cout <<"\nentering: " << entering << "\nleaving: " << leaving; \
		std::cout << std::endl; \
	}); \
	__VA_OPT__(__VA_ARGS__();) \
	simplex.optimize_safe(); \
 \
	return (SOL) != simplex.f(); \
} \

#endif // !_CALGO_OPT_TRANSPORTATION_SIMPLEX_TESTING_TEST_HPP_
