#include <calgo/in/cubicParametric.hpp>
#include <calgo/lin/cyclicThomas.hpp>

#include <cmath>

namespace ca::in {

void CubicParametric::setX(ListT x) {
	ParametricInterpolation::setX(x);
	calculateMs();
}

void CubicParametric::calculateMs() {
	ListT a(i_n, 1.0/6.0), b(i_n, 2.0/3.0), fx(i_n), fy(i_n); // c = a
	
	fx[0] = i_x[1] - 2*i_x[0] + i_x[i_n-1];
	fy[0] = i_y[1] - 2*i_y[0] + i_y[i_n-1];
	fx[i_n-1] = i_x[0] - 2*i_x[i_n-1] + i_x[i_n-2];
	fy[i_n-1] = i_y[0] - 2*i_y[i_n-1] + i_y[i_n-2];
	for (std::size_t i = 1; i < i_n - 1; i++) {
		fx[i] = i_x[i+1] - 2*i_x[i] + i_x[i-1];
		fy[i] = i_y[i+1] - 2*i_y[i] + i_y[i-1];
	}

	n_Mx = ca::lin::CyclicThomas::solve(a, b, a, fx); // c = a
	n_My = ca::lin::CyclicThomas::solve(a, b, a, fy); // c = a
}

double CubicParametric::calculateS(double t, ListT& m, ListT& f) {
	long long i = static_cast<long long>(t);
	i %= (i_n);
	t = std::fmod(t, i_n);
	long long ti = i+1;

	if (i == 0)
		return m[i_n-1]*std::pow(ti - t, 3)/6 +
		m[i]*std::pow(t - (ti-1), 3)/6 + 
		(f[i_n-1] - m[i_n-1]/6)*(ti - t) +
		(f[i] - m[i]/6)*(t - (ti-1));
	
	return m[i-1]*std::pow(ti - t, 3)/6 +
	m[i]*std::pow(t - (ti-1), 3)/6 + 
	(f[i-1] - m[i-1]/6)*(ti - t) +
	(f[i] - m[i]/6)*(t - (ti-1));
}
	
}
