#include <iostream> 
#include <cmath> 
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<int> m = {
		{ 4, 5, 7 },
		{ 3, 9, 2 },
		{ 1, 6, 8 },
	};
	ca::mat<double> expected = {
		{ 60.0/193,  2/193.0,   -53/193.0 },
		{ -22/193.0, 25/193.0,  13/193.0  },
		{ 9/193.0,   -19/193.0, 21/193.0  },
	};
	ca::mat<double> inverse = m.inverse_safe<double>();
	std::cout << "inv: " << &inverse << std::endl;
	std::cout << "inv data: " << inverse.data() << std::endl;

	std::cout << "got:\n" << inverse << "expected:\n" << expected;

	return not inverse.compare(expected, [](const double& a, const double& b) { return std::abs(a - b) < 0.1; });
}
