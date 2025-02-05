#ifndef _CALGO_LIN_TESTING_TEST_HPP_
#define _CALGO_LIN_TESTING_TEST_HPP_

#include <calgo/vec.hpp>
#include <calgo/mat.hpp>
#include <calgo/lin/gauss.hpp>

#include <cmath>
#include <string>
#include <iostream>
#include <functional>

template<typename T>
bool comp(
	const ca::vec_view<T> a, 
	const ca::vec_view<T> b, 
	double delta = 0.05) {
	if (a.n() != b.n())
		return false;

	for (typename ca::vec<T>::size_type i = 0; i < a.n(); i++)
		if (std::abs(a[i] - b[i]) > delta)
			return false;

	return true;
}

#define CA_LIN_TEST_GAUSS(SYS, ...) \
int main() { \
	SYS s; \
	__VA_OPT__(__VA_ARGS__(s);) \
	s.setConstraints(constraints); \
	s.setVariables(variables); \
	s.initCallback([](auto vars, auto constr) { \
		std::cout << "INIT:" << std::endl  \
			<< "system:" << std::endl; \
		vars.showSystem(constr); \
	}); \
	s.iterationCallback([](auto vars, auto constr, auto i) { \
		std::cout << "\n\nITERATION №" << i << std::endl  \
			<< "system:" << std::endl; \
		vars.showSystem(constr); \
	}); \
	s.swapCallback([](auto vars, auto constr, auto row, auto i) { \
		std::cout << "\n\nSWAP on row " << row <<  " (IT №" << i << ')'<< std::endl  \
			<< "system:" << std::endl; \
		vars.showSystem(constr); \
	}); \
	s.outCallback([](auto vars, auto constr, auto sol, auto i) { \
		std::cout << "\n ----  SOL (backward move) №" << i << std::endl \
			<< "sol: " << sol << std::endl << "system:" << std::endl; \
		vars.showSystem(constr); \
	}); \
	s.solve(); \
	std::cout << "\n\nSYSTEM: \n"; variables.showSystem(constraints) << std::endl; \
	std::cout << "sol:  " << s.solution() << std::endl; \
	std::cout << "esol: " << solution << std::endl; \
	return not comp(s.solution(), solution); \
} \

#endif // !_CALGO_LIN_TESTING_TEST_HPP_
