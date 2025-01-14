#include <iostream>
#include <calgo/vec.hpp>

int main(int argc, char** argv) {
	ca::Vec<double> v(10);

	std::cout << "empty: " << v << std::endl;

	v.resize(10);

	std::cout << "10 elems: " << v << std::endl;

	v[1] = 69;

	std::cout << "10 elems (second changed): " << v << std::endl;

	return 0;
}
