#pragma once
#include <calgo/num/boundary_value_problem.hpp>

#include <calgo/lin/thomas.hpp>

#include <stdexcept>

namespace ca::num {

template<typename T>
void boundary_value_problem<T>::check() {
	if (not m_nodes)
		throw std::runtime_error("ca::nm::boundary_value_problem: no nodes");
	if (not m_f)
		throw std::runtime_error("ca::nm::boundary_value_problem: no f(x))");
	if (not m_q)
		throw std::runtime_error("ca::nm::boundary_value_problem: no q(x)");
}

template<typename T>
void boundary_value_problem<T>::solve() {
	const auto n = m_nodes->n();

	vec<value_type> ones(n-2);
	ones.set(1);
	vec<value_type> b(n-1), f(n-1);

	for (typename vec<value_type>::size_type i = 0; i < n-1; i++) {
		auto h_squared = std::pow(m_nodes->weight(i), 2);
		b[i] = -(2 + h_squared*m_q(m_nodes->nodes()[i]));
		f[i] = -m_f(m_nodes->nodes()[i])*h_squared;
	}
	f[0] -= m_mu.first;
	f[n-2] -= m_mu.second;

	ca::lin::thomas<value_type> thomas;
	thomas.set_a(ones); 
	thomas.set_b(b);
	thomas.set_c(ones);
	thomas.set_constraints(f);
	thomas.solve();

	m_y.resize(n+1);
	m_y[0] = m_mu.first;
	m_y[n] = m_mu.second;
	for (typename vec<value_type>::size_type i = 1; i <= n; i++)
		m_y[i] = thomas.solution()[i-1];
}

}
