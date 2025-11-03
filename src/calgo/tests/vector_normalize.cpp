#include <calgo/vec.hpp> 
#include <iostream> 

int main(int argc, char** argv) {
	ca::vec<double> v1 = { 9, 2, 11 };
	v1.normalize<double>();
	std::cout << "len: " << v1.length() << std::endl;

	return v1.length() != 1;
}
