#include <calgo/num/fibonachiMinimum.hpp>

#include <iostream>

int main(int argc, char** argv) {
	using namespace ca::num;
	auto fx = [](auto x) {
			return -x/(1.2 - std::sin(2*x));
		};

	FibonachiMinimum<double> f;
	f.setFunction(fx);
	f.setInterval({-1, 1});
	f.setEpsilon(0.0001);
	f.setIterations(20);
	f.iterationCallback(
		[](auto inter, auto it) {
			std::cout << "iteration " << it << " -> " << inter << std::endl;
		}
	);

	f.find_safe();

	std::cout << '(' << f.x() << ',' << fx(f.x()) << ')' << std::endl;

	return not ((f.x() - 0.846) < 0.1);
}
