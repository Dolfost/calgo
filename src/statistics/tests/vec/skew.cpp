#include <calgo/ss/vec.hpp>
#include <iostream>

int main(int argc, char** argv) {
	ca::ss::vec<double> v{5, 3, 6, 7, 3, 6, 8, 3};
	double m = v.skew().biased;
	double e = 0.0551899;

	std::cout << "expected: " << e << std::endl <<
		"got:      " << m << std::endl;

	return std::abs(m - e) >= 0.1;
}
