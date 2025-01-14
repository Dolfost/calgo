#include <iostream>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::Mat<double> m(5, 5);

	std::cout << "empty:\n" << m << std::endl;

	std::cout << "5x5 elems:\n" << m << std::endl;

	m[1][1] = 1;
	std::cout << "5x5 elems ((1,1) changed):\n" << m << std::endl;

	m(2, 2) = 2;
	std::cout << "5x5 elems ((2,2) changed):\n" << m << std::endl;

	return 0;
}
