#include <calgo/lin/cyclicThomas.hpp>

namespace ca::lin {

ListT CyclicThomas::solve(
		const ListT& a,
		const ListT& c,
		const ListT& b,
		const ListT& f
) {
	std::size_t N = c.size();

	ListT al(N), bt(N), gm(N);
	al[0] = -b[0]/c[0];
	bt[0] = f[0]/c[0];
	gm[0] = -a[0]/c[0];

	for (int i = 1; i < N-1; i++) {
		double zi = -c[i]-a[i]*al[i-1];
		al[i] = b[i]/zi;
		bt[i] = (-f[i]+a[i]*bt[i-1])/zi;
		gm[i] = a[i]*gm[i-1]/zi;
	}

	ListT p(N-1), q(N-1);
	p.back() = bt[N-2];
	q.back() = al[N-2] + gm[N-2];

	for (int i = N - 3; i >= 0; i--) {
		p[i] = al[i]*p[i+1] + bt[i];
		q[i] = al[i]*q[i+1] + gm[i];
	}

	ListT x(N);
	x.back() = (-f.back() + b.back()*p[0] + a.back()*p.back()) /
		(-c.back() - b.back()*q[0] - a.back()*q.back());
	for (int i = 0; i < N-1; i++)
		x[i] = p[i] + x.back()*q[i];

	return x;
}

}

