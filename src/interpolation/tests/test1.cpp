#include <iomanip>
#include <iostream>

#include <calgo/in/interpolation.hpp>
#include <calgo/vec.hpp>

bool comp(
	const ca::vec<double>& a, 
	const ca::vec<double>& b, 
	double delta = 0.01) {
	if (a.n() != b.n())
		return false;

	for (std::size_t i = 0; i < a.n(); i++)
		if (std::abs(a[i] - b[i]) > delta)
			return false;

	return true;
}

int main() {
	ca::vec<double>	expect = {0.245, 2.06, 5, 7.94, 9.76};
	ca::in::ChebyshevNodes<double> n(0, 10, 5);

	std::cout << "expected: ";
	for (std::size_t i = 0; i < expect.n(); i++) {
		std::cout << std::setw(6) << std::setprecision(3) << expect[i] << " ";
	}
	std::cout << "\nres:      ";
	for (std::size_t i = 0; i < n.nodes().n(); i++) {
		std::cout << std::setw(6) << std::setprecision(3) << n.nodes()[i] << " ";
	}

	return !comp(n.nodes(), expect);
}
