#include <iomanip>
#include <iostream>

#include <calgo/interpol/interpol.hpp>

bool comp(
	const std::vector<double>& a, 
	const std::vector<double>& b, 
	double delta = 0.01) {
	if (a.size() != b.size())
		return false;

	for (auto ait = a.begin(), bit = b.begin(); ait != a.end(); ait++, bit++)
		if (std::abs(*ait - *bit) > delta)
			return false;

	return true;
}

int main() {

	std::vector<double> 
	sol = ca::interpol::Nodes::chebyshev(0, 10, 5),
	expect = {0.245, 2.06, 5, 7.94, 9.76};

	std::cout << "expected: ";
	for (auto const& x : expect) {
		std::cout << std::setw(6) << std::setprecision(3) << x << " ";
	}
	std::cout << "\nres:      ";
	for (auto const& x : sol) {
		std::cout << std::setw(6) << std::setprecision(3) << x << " ";
	}

	return !comp(sol, expect);
}
