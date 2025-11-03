#include <iostream> 
#include <cmath> 
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<int> m = {
		{ 4, 5, 7 },
		{ 3, 9, 2 },
		{ 1, 6, 8 },
	};
	ca::mat<int> expected = {
		{ 4, 3, 1 },
		{ 5, 9, 6 },
		{ 7, 2, 8 }
	};
	ca::mat<double> transpose = m.transpose();
	std::cout << "got:\n" << transpose << "expected:\n" << expected;

	return transpose != expected;
}
