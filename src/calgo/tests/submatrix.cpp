#include <iostream>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<double> m = {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9, 10},
		{11, 12, 13, 14, 15},
		{16, 17, 18, 19, 20},
		{21, 22, 23, 24, 25},
	};

	std::cout << "mat 5x5:\n" << m;
	std::cout << "\nsub(0,0,5,5):\n" << m.submat(0,0, 5, 5);
	std::cout << "\nsub(1,1,4,3):\n" << m.submat(1,1, 4, 3);


	return 0;
}
