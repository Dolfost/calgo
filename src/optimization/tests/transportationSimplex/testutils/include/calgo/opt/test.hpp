#ifndef _CALGO_OPT_TRANSPORTATION_SIMPLEX_TESTING_TEST_HPP_
#define _CALGO_OPT_TRANSPORTATION_SIMPLEX_TESTING_TEST_HPP_

#include <calgo/opt/transportationSimplex.hpp>

// headers required in the tests
#include <iostream>
#include <cmath>

#define CA_OPT_TRANSPORTATION_SIMPLEX_TEST(SOL, ...) \
int main(int argc, char** argv) { \
 \
	ca::Vec<double> f = demand; \
	ca::Vec<double> c = supply; \
	ca::Mat<double> v = cost; \
	std::cout <<"INPUT:\n"; \
	v.showSystem(c) << '\n' << f << std::endl << std::endl; \
	ca::opt::TransportationSimplex<double> simplex; \
	simplex.setDemand(f); \
	simplex.setSupply(c); \
	simplex.setCost(v); \
	simplex.iterationCallback([&simplex](auto ctx, auto it) { \
		std::cout << "--->> ITEEATION No " << it << std::endl \
			<< simplex << "\ndistribution:\n" << ctx.distribution << std::endl; \
	}); \
	__VA_OPT__(__VA_ARGS__();) \
	simplex.optimize_safe(); \
 \
	std::cout << "\nResult:\n" << simplex << std::endl; \
 \
	return 0; \
} \

#endif // !_CALGO_OPT_TRANSPORTATION_SIMPLEX_TESTING_TEST_HPP_
