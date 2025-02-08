#include <calgo/ss/vec.hpp>
#include <calgo/ss/statistic.hpp>

int main(int argc, char** argv) {
	ca::ss::vec<double> v(5);
	std::cout << v << std::endl;
	v.at(0) = 1; v.at(2) = 10;
	std::cout << v.raw_moment(1) << std::endl;

	ca::vec<double> vv = {2,4,5,6,3,3};
	ca::ss::vec<double> svv(vv);

	v.revoke();

	std::cout << svv;

	return 0;
}
