#pragma once
#include <calgo/num/euler_cauchy.hpp>

#include <stdexcept>

namespace ca::num {

template<typename T>
void euler_cauchy<T>::check() {
	if (not m_nodes)
		throw std::runtime_error("ca::nm::euler_cauchy: no nodes");
	if (not m_f)
		throw std::runtime_error("ca::nm::euler_cauchy: no f(t, u(t))");
}

template<typename T>
void euler_cauchy<T>::solve() {
	m_y.resize(m_nodes->n());
	m_y[0] = m_u0;
	for (std::size_t i = 1; i < m_nodes->n(); i++) {
		value_type f_tn_yn = m_f(m_nodes->nodes()[i-1], m_y[i-1]);
		value_type tau = m_nodes->weight(i-1);
		m_y[i] = m_y[i-1] + tau*f_tn_yn;
		m_y[i] = m_y[i-1] + tau*(f_tn_yn + m_f(m_nodes->nodes()[i], m_y[i]))/2;
	}
}

}
