#pragma once
#include <calgo/num/boundary_value_problem.hpp>

#include <calgo/lin/thomas.hpp>

#include <stdexcept>

namespace ca::num {

template<typename T, bool k_present>
void bvp<T, k_present>::check() {
	if (not m_nodes)
		throw std::runtime_error("ca::nm::boundary_value_problem: no nodes");
	if (not m_f)
		throw std::runtime_error("ca::nm::boundary_value_problem: no f(x))");
	if (not m_q)
		throw std::runtime_error("ca::nm::boundary_value_problem: no q(x)");
	if constexpr (k_present)
		if (not this->m_k)
			throw std::runtime_error("ca::nm::boundary_value_problem with k(x): no k(x)");
}

template<typename T, bool k_present>
void bvp<T, k_present>::solve() {
	const auto n = m_nodes->n()-1; // intervals count
	vec<typename bvp_traits<T>::value_type> b(n-1), f(n-1);
	ca::lin::thomas<typename bvp_traits<T>::value_type> thomas;

	if constexpr (k_present) {
		vec<typename bvp_traits<T>::value_type> a(n-2), c(n-2);

		{
			typename vec<typename bvp_traits<T>::value_type>::size_type i;
			for (i = 0; i < n-2; i++) {
				a[i] = this->m_k(m_nodes->nodes()[i+2] - m_nodes->weight(i+2)/2)/m_nodes->weight(i+2);
				b[i] = -(this->m_k(m_nodes->nodes()[i+1]+m_nodes->weight(i+1)/2) +
					this->m_k(m_nodes->nodes()[i+1]-m_nodes->weight(i+1)/2))/m_nodes->weight(i+1) -
					m_q(m_nodes->nodes()[i+1])*m_nodes->weight(i+1);
				c[i] = this->m_k(m_nodes->nodes()[i+1] + m_nodes->weight(i+1)/2)/m_nodes->weight(i+1);
				f[i] = -m_nodes->weight(i+1)*m_f(m_nodes->nodes()[i+1]);
			}
			b[i] = -(this->m_k(m_nodes->nodes()[i+1]+m_nodes->weight(i+1)/2) +
				this->m_k(m_nodes->nodes()[i+1]-m_nodes->weight(i+1)/2))/m_nodes->weight(i+1) -
				m_q(m_nodes->nodes()[i+1])*m_nodes->weight(i+1);
			f[i] = (-m_nodes->weight(i+1)*m_f(m_nodes->nodes()[i+1]) - 
				this->m_k(m_nodes->nodes()[i+1] + m_nodes->weight(i+1)/2)/m_nodes->weight(i+1))*m_mu.second;
			f[0] -= (this->m_k(m_nodes->nodes()[1] - m_nodes->weight(1)/2)/m_nodes->weight(1))*m_mu.first;
		}

		thomas.set_a(a); 
		thomas.set_b(b);
		thomas.set_c(c);
		thomas.set_constraints(f);
		thomas.solve();
	} else { // no k(x)
		vec<typename bvp_traits<T>::value_type> ones(n-2);
		ones.set(1);

		for (typename vec<typename bvp_traits<T>::value_type>::size_type i = 0; i < n-1; i++) {
			auto h_squared = std::pow(m_nodes->weight(i+1), 2);
			b[i] = -(2 + h_squared*m_q(m_nodes->nodes()[i+1]));
			f[i] = -m_f(m_nodes->nodes()[i+1])*h_squared;
		}
		f[0] -= m_mu.first;
		f[n-2] -= m_mu.second;

		thomas.set_a(ones); 
		thomas.set_b(b);
		thomas.set_c(ones);
		thomas.set_constraints(f);
		thomas.solve();
	}

	m_y.resize(n+1);
	m_y[0] = m_mu.first;
	m_y[n] = m_mu.second;
	for (typename vec<typename bvp_traits<T>::value_type>::size_type i = 1; i < n; i++)
		m_y[i] = thomas.solution()[i-1];
}

}
