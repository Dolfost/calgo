#include <iostream>
#include <calgo/vec.hpp>

int main(int argc, char** argv) {
	ca::Vec<double> v = {1, 2, 3};
	ca::VecView<double> vv = v;

	std::cout << "v    : " << v << std::endl;
	std::cout << "view : " << vv << std::endl << std::endl;

	ca::Vec<double> v2 = std::move(v);
	std::cout << "moved: " << v2 << std::endl;
	std::cout << "v.n(): " << v.n() << std::endl;

	return 0;
}