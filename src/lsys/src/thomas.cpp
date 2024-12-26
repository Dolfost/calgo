#include "calgo/lsys/thomas.hpp"

namespace ca::lsys {

ListT Thomas::solve(
		const ListT& a,
		const ListT& b,
		const ListT& c,
		const ListT& d
) {
	std::size_t N = b.size();

	ListT c_star(N);
	ListT d_star(N);

	c_star[0] = c[0] / b[0];
	d_star[0] = d[0] / b[0];

	for (int i = 1; i < N; i++) {
		double m = (b[i] - a[i-1]*c_star[i-1]);
		c_star[i] = c[i]/m;
		d_star[i] = (d[i] - a[i-1]*d_star[i-1])/m;
	}

	ListT x(N);
	x.back() = d_star.back();
	for (int i = N-2; i >= 0; i--) {
		x[i] = d_star[i] - c_star[i] * x[i+1];
	}
	return x;
}

}

