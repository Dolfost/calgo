#include <calgo/num/simpson.hpp>
#include <iostream>
#include <cmath>

int main(int argc, char** argv) {
	ca::num::Simpson s;
	// sinx-cos2x+2
	s.setX({-1, 0, 1, 2, 3, 4, 5, 6});
	s.setY({1.574, 1, 3.257, 3.562, 1.180, 1.388, 1.880, 0.876});

	double i = s.integrate();
	double ie = 13.3937;
	std::cout << "Integral: " << i << std::endl;
	std::cout << "Expected: " << ie << std::endl;

	return std::abs(i - ie) > 0.8;
}
