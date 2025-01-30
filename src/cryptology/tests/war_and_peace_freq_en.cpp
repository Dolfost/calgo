#include <calgo/cr/freq_analysis.hpp> 
#include <iostream>
#include <numeric>

int main(int argc, char** argv) {
	ca::cr::freq a(std::filesystem::path(TESTPATH "/war_and_peace_en.txt"));
	std::cout << a;

	double s = 0;
	for (auto const& x : a) {
		s += x.second;
	}
	
	std::cout << "sum: " << s << std::endl;

	return 0;
}
