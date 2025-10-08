#include <iostream>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<double> a = {
		{ 5,  6  },
		{ -1, 4 },
		{ 2,  3  },
	};
	ca::vec<int> b = { 4,  0 };
	auto res = a.mul_safe(b);
	ca::vec<double> expected = { 20, -4, 8 };

	std::cout << "got:      " << res << "\nexpected: " << expected << std::endl;

	return res != expected;
}
