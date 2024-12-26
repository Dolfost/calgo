#include <iomanip>
#include <iostream>
#include <calgo/lsys/cyclicThomas.hpp>

bool comp(
	const std::vector<double>& a, 
	const std::vector<double>& b, 
	double delta = 0.05) {
	if (a.size() != b.size())
		return false;

	for (auto ait = a.begin(), bit = b.begin(); ait != a.end(); ait++, bit++)
		if (std::abs(*ait - *bit) > delta)
			return false;

	return true;
}

int tcount = 0;

bool fancyComp(const ca::ListT& roots, const ca::ListT& expected) {
	bool cmp = comp(roots, expected);
	std::string pref = cmp ? "OK" : "BAD";
	std::cout << "\n[" << pref << "] expected: ";
	for (auto const& x : expected) {
		std::cout << std::setw(7) << std::setprecision(3) << x << " ";
	}
	std::cout << "\n[" << pref << "] res:      ";
	for (auto const& x : roots) {
		std::cout << std::setw(7) << std::setprecision(3) << x << " ";
	}
	std::cout << std::endl;

	tcount++;
	return cmp;
}

int main() {
	int r = 0;

	r += fancyComp(
		ca::lsys::CyclicThomas::solve(
			{5, 4, 5, 6, 9},  // a
			{6, 8, 5, 3, 11}, // b
			{9, 7, 3, 1, 5},  // c
			{7, 3, 9, 4, 5}   // d
		),
		{-3.664, 1.677, 0.606, -0.805, 2.779}
	); 

	r += fancyComp(
		ca::lsys::CyclicThomas::solve(
			{5, 3, 7, 9, 2, 4},
			{2, 1, 5, 7, 3, 9},
			{5, 1, 3, 9, 0, 3},
			{7, 1, 3, 6, 7, 9}
		),
		{1.087, -0.061, -2.201, 4.81, -0.873, 1.026}
	);

	r += fancyComp(
		ca::lsys::CyclicThomas::solve(
			{9, 3, 7, 9, 2, 4},
			{2, 3, 5, 7, 4, 9},
			{5, 1, 3, 9, 0, 3},
			{7, 1, 3, 6, 7, 9}
		),
		{-1.440, 1.867, -0.281, -2.887, 3.193, 0.061}
	);

	return !(r == tcount);
}
