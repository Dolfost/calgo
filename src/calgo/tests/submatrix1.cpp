#include <iostream>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<double> m = {
		{-2, 3, 0, 0, 0, 0},
		{-1, 2, -1, 0, 0, 2},
		{-5, 2, 0, 1, 0, 10},
		{1, 2, 0, 0, -1, 4},
	};

	std::cout << "mat 4x6:\n" << m 
		<< "\nmat(1, 0, r-1, c-1):\n" << m.submat(
			1, 0, m.rows() - 1,
			m.cols() - 1
		);


	return 0;
}
