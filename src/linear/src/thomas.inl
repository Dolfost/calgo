#pragma once
#include <calgo/lin/thomas.hpp>

namespace ca::lin {

template<typename T>
void Thomas<T>::init() {
	this->m_solution.resize(m_constraints.n());
	this->m_solution.set(0);
}

template<typename T>
void Thomas<T>::solve() {
	init();
	const auto N = m_b.n();

	vec<value_type> c_star(N);
	vec<value_type> d_star(N);

	c_star[0] = m_c[0] / m_b[0];
	d_star[0] = m_constraints[0] / m_b[0];

	for (int i = 1; i < N; i++) {
		double m = (m_b[i] - m_a[i-1]*c_star[i-1]);
		c_star[i] = m_c[i]/m;
		d_star[i] = (m_constraints[i] - m_a[i-1]*d_star[i-1])/m;
	}

	this->m_solution[N-1] = d_star[N-1];
	for (int i = N - 2; i >= 0; i--)
		this->m_solution[i] = d_star[i] - c_star[i] * this->m_solution[i+1];
}

template<typename T>
void Thomas<T>::check() {
	if (m_b.n() != m_constraints.n())
		throw std::runtime_error(
			"ca::Thomas: number of constraints "
			"is not equal to number of diagonal elements"
		);
	if (m_a.n() != m_c.n())
		throw std::runtime_error(
			"ca::Thomas: size of A is not equal to size of C"
		);
	if (m_a.n() != m_b.n() - 1)
		throw std::runtime_error(
			"ca::Thomas: size of A is not equal to B-1"
		);
};

}

