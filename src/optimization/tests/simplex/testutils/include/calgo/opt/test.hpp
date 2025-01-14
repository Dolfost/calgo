#ifndef _CALGO_OPT_SIMPLEX_TESTING_TEST_HPP_
#define _CALGO_OPT_SIMPLEX_TESTING_TEST_HPP_

#include <calgo/opt/simplex.hpp>

// headers required in the tests
#include <iostream>
#include <cmath>

#define CA_OPT_SIMPLEX_TEST(SOL, ...) \
int main(int argc, char** argv) { \
 \
	ca::Vec<double> f = func; \
	ca::Vec<double> c = constr; \
 \
	ca::Mat<double> v = vals; \
 \
	ca::opt::Simplex<double> simplex(&v, &c, &f); \
	simplex.validCallback([](auto ctx) { \
		std::cout << "Input tableau:\n"; \
		ctx.vars->showSystem(*ctx.constr, std::cout); \
		std::cout << "func: " << *ctx.func << std::endl << std::endl; \
	}); \
	\
	simplex.initCallback([](auto ctx) { \
		std::cout << "Initial tableau:\n"; \
		ctx.vars->showSystem(*ctx.constr, std::cout); \
		std::cout << "basis: " << ctx.bas << std::endl; \
		std::cout << "func: " << *ctx.func << std::endl; \
		std::cout << "netEval: " << ctx.netEval << std::endl; \
	}); \
 \
	simplex.pivotCallback([](auto ctx, auto row, auto col) { \
		static size_t no = 0; \
		std::cout << "\nPivot №" << ++no << " at (" << row << "," << col << ")\n"; \
		std::cout <<  *ctx.func << "  <--- function" << std::endl; \
		ctx.vars->showSystem(*ctx.constr, std::cout); \
		std::cout << ctx.netEval << "  <--- net evaluation" << std::endl; \
		std::cout << "function estimate: " <<  ctx.f << std::endl; \
		if (no >= 120) { \
			std::cout << "\n\nOver 120 made iterations. Stopping...\n";\
			exit(1); \
		} \
	}); \
 \
	__VA_OPT__(__VA_ARGS__();) \
	simplex.optimize(); \
 \
	std::cout << "\nResult:\n" << simplex << std::endl; \
 \
	return std::abs(simplex.f() - SOL) > 0.001; \
} \

#endif // !_CALGO_OPT_SIMPLEX_TESTING_TEST_HPP_
