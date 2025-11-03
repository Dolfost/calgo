#include <calgo/vec.hpp> 
#include <cmath>

int main(int argc, char** argv) {
	ca::vec<int> v1 = { 4, 5, 9 };
	ca::vec<double> v2 = { 9, 2, 11 };

	auto l1 = v1.length<double>();
	auto l2 = v2.length();

	return (l1 != std::sqrt(122)) or (l2 != std::sqrt(206)); 
}
