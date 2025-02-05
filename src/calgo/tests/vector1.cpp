#include <iostream>
#include <calgo/vec.hpp>

int main(int argc, char** argv) {
	ca::vec<double> v = {1, 2, 3};
	ca::vec<double> v1 = v;
	ca::vec_view<double> vv = v1;
	ca::vec<double> v2 = vv;

	std::cout << "empty: " << &v << " " << v << std::endl;
	std::cout << "v1   : " << &v1 << " " << v1 << std::endl;
	std::cout << "view : " << &vv << " " << vv << std::endl;

	return 0;
}
