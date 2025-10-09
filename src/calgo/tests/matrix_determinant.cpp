#include <iostream>
#include <calgo/mat.hpp>

int main() {
	ca::mat<int> m = {
		{ 4, 5, 7 },
		{ 3, 9, 2 },
		{ 1, 6, 8 },
	};
	double expected = 193;
	double determinant = m.det<double>();
	std::cout << "expected: " << expected << "\ngot:      " << determinant << std::endl;

	return expected != determinant;
}
