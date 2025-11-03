#include <calgo/vec.hpp> 
#include <cmath>

int main(int argc, char** argv) {
	ca::vec<int> v1 = { 4, 5, 9 };
	ca::vec<double> v2 = { 9, 2, 11 };
	auto v = v1 - v2;
	return v != ca::vec<int>({ -5, 3, -2 });
}
