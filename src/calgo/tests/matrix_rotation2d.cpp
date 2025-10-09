#include <iostream>
#include <calgo/calgo.hpp>
#include <calgo/mat.hpp>

int main(int argc, char** argv) {
	ca::mat<double> a(2, 2);
	a.rotation2d(ca::mconst.pi);
	std::cout << a;

	return 0;
}
