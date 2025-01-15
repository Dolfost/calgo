#include <iostream>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::Mat<double> m(5, 5);
	ca::Vec<double> v = {1, 2, 3, 4, 5};

	std::cout << "empty 5x5:\n" << m << std::endl;

	m[1][1] = 1;
	std::cout << "5x5 elems ((1,1) changed):\n" << m << std::endl;

	m(2, 2) = 2;
	std::cout << "5x5 elems ((2,2) changed):\n" << m << std::endl;

	m(1, 3) = 2;
	std::cout << "5x5 elems ((1,3) changed):\n" << m << std::endl;

	for (ca::Mat<double>::size_type i = 0; i < m.cols(); i++)
		std::cout << "col " << i << ": " << m.col(i) << std::endl;
	std::cout << std::endl;
	for (ca::Mat<double>::size_type i = 0; i < m.rows(); i++)
		std::cout << "row " << i << ": " << m.row(i) << std::endl;

	std::cout << std::endl;
	m.showSystem(v);

	std::cout << "\ncasting to const... " << std::endl;
	auto m1 = static_cast<const ca::Mat<double>&>(m);
	for (ca::Mat<double>::size_type i = 0; i < m1.rows(); i++)
		std::cout << "row " << i << ": " << m1.row(i) << std::endl;

	std::cout << std::endl;
	m1.showSystem(v);


	return 0;
}
