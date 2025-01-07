#include "calgo/num/simpson.hpp"

namespace ca::num {

double Simpson::integrate() {
	std::size_t n = s_x.size() - 1;

	double step = h(0);

	double odd = 0.0;
	for (int i = 1; i < n; i += 2)
		odd += s_y[i];

	double even = 0.0;
	for (int i = 2; i < n; i += 2)
		even += s_y[i];

	return (s_y.front() + s_y.back() + 2 * even + 4 * odd) * step / 3;
}

}
