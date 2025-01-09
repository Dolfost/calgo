#ifndef _CALGO_OPT_SIMPLEX_TESTING_TEST_HPP_
#define _CALGO_OPT_SIMPLEX_TESTING_TEST_HPP_

#include<calgo/opt/simplex.hpp>

#include<iostream>

#define CA_OPT_SIMPLEX_TEST(SOL) \
int main(int argc, char** argv) { \
 \
	ca::Vector<double> f = func; \
	ca::Vector<double> c = constr; \
 \
	ca::Matrix<double> v = vals; \
 \
	ca::opt::Simplex<double> simplex(&v, &c, &f); \
	simplex.initCallback([](auto ctx) { \
		std::cout << "Initial tableau:\n"; \
		ctx.vars->system(*ctx.constr, std::cout); \
		std::cout << "basis: " << ctx.bas << std::endl; \
		std::cout << "func: " << *ctx.func << std::endl; \
		std::cout << "unitCon: " << ctx.unitCon << std::endl; \
		std::cout << "netEval: " << ctx.netEval << std::endl; \
	}); \
 \
	simplex.pivotCallback([](auto ctx, auto row, auto col) { \
		static size_t no = 0; \
		std::cout << "\nPivot №" << ++no << " at (" << row << "," << col << ")\n"; \
		std::cout <<  *ctx.func << "  <--- function" << std::endl; \
		ctx.vars->system(*ctx.constr, std::cout); \
		std::cout << ctx.unitCon << "  <--- unit contribution" << std::endl; \
		std::cout << ctx.netEval << "  <--- net evaluation" << std::endl; \
		std::cout << "funciton estimate: " <<  ctx.f << std::endl; \
		if (no >= 120) { \
			std::cout << "\n\nOver 120 made iterations. Stopping...\n";\
			exit(1); \
		} \
	}); \
 \
	simplex.optimize(); \
 \
	std::cout << "\nResult:\n" << simplex << std::endl; \
 \
	return (simplex.f() - SOL) > 0.001; \
} \

#endif // !_CALGO_OPT_SIMPLEX_TESTING_TEST_HPP_
