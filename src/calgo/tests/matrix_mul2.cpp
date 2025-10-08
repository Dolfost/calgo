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
		{ 10, 1, 4 }, 
		{ 10, 1, 4 }, 
	};
	ca::mat<double> res;
	try {
		res = a.mul_safe(b);
	} catch (std::logic_error& ex) {
		std::cout << "Matrices are non-conformant. success\n";
		return 0;
	}
	return 1;
}
