#include <calgo/vec.hpp> 
#include <iostream> 

int main(int argc, char** argv) {
	ca::vec<double> v1 = { 9, 2, 11 };
	ca::vec<int> v2 = { 2, 7, 3 };
	auto cross = v1.cross_3d(v2);

	std::cout << "v1: " << v1 << "\nv2: " << v2 << "\ncross: " << cross << std::endl;

	return cross != ca::vec<int>{ -71, -5, 59 };
}
