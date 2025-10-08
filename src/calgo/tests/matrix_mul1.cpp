#include <iostream>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<double> a = {
		{ 5,  6  },
		{ -1, 4 },
		{ 2,  3  },
		{ 7,  5  },
	};
	ca::mat<int> b = {
		{ 4,  0, 8 }, 
		{ 10, 1, 4 }, 
	};
	ca::mat<float> b_float = {
		{ 4,  0, 8 }, 
		{ 10, 1, 4 }, 
	};
	auto res = a.mul_safe(b);
	ca::mat<double> expected = {
		{ 80, 6, 64 },
		{ 36, 4, 8  },
		{ 38, 3, 28 },
		{ 78, 5, 76 },
	};

	std::cout << "got:\n" << res << "expected:\n" << expected;

	a.mul_safe(b_float);

	return res != expected;
}
