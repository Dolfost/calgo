#include <calgo/num/boundingPhase.hpp>

#include <iostream>
#include <cmath>

int main(int argc, char** argv) {
	using namespace ca::num;

	BoundingPhase<double> bp;
	bp.setFunction([](auto x) { return 3*std::sin(x/2+2.17); });
	bp.set_starting_point(8.3);
	bp.set_step(0.1);
	bp.iterationCallback([](auto x0, auto x1, auto x2, auto f0, auto f1, auto f2, auto it) {
		std::cout << "iteraton " << it << std::endl <<
		"x0 " << x0 << "\nx1 " << x1 << "\nx2 " << x2 << "\nf0 " << f0 << "\nf1 " << f1 << "\nf2 "
			<< f2 << std::endl;
	});

	bp.localize_safe();

	std::cout << std::boolalpha << "found: " << bp.found() << "\nint: " << bp.local_interval() << std::endl;

	return not (bp.local_interval().a < 3 and bp.local_interval().b > 6);
}
