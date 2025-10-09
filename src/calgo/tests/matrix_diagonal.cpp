#include <iostream>
#include <calgo/mat.hpp>

int main() {
	ca::mat<int> m = {
		{ 5,  3,  6,  9  },
		{ 12, 10, 13, 16 },
		{ 14, 13, 11, 19 },
		{ 17, 19, 43, 15 },
	};
	ca::vec<int> expected = { 5, 10, 11, 15 };

	std::cout << "expected: " << expected << "\ngot:      " << m.diagonal() << std::endl;

	return m.diagonal() != expected;
}
