#include <calgo/ss/vec.hpp>

int main(int argc, char** argv) {
	ca::ss::vec<double> v{5, 3, 6, 7, 3, 6, 8, 3};
	double m = v.raw_moment(1);
	double e = 5.125;

	std::cout << "expected: " << e << std::endl <<
		"got:      " << m << std::endl;

	return std::abs(m - e) >= 0.1;
}
