#include <iostream>
#include <calgo/mat.hpp>

int main() {
	ca::mat<int> m = {
		{ 5,  3,  6,  9,  18 },
		{ 12, 10, 13, 16, 13 },
		{ 14, 13, 11, 19, 7  },
		{ 17, 19, 43, 15, 9  },
	};
	ca::vec<int> expected1 = { 5, 10, 11, 15 };
	ca::vec<int> expected2 = { 12, 13, 43 };
	ca::vec<int> expected3 = { 3, 13, 19 };

	std::cout << "expected: " << expected1 << "\ngot:      " << m.diagonal() << std::endl << std::endl;
	std::cout << "expected: " << expected2 << "\ngot:      " << m.diagonal_lower_safe(1) << std::endl << std::endl;
	std::cout << "expected: " << expected3 << "\ngot:      " << m.diagonal_upper(1) << std::endl;

	return not ((m.diagonal() == expected1) and (m.diagonal_lower_safe(1) == expected2) and (m.diagonal_upper_safe(1) == expected3));
}
