#pragma once
#include <calgo/num/runge_kutta.hpp>

#include <stdexcept>

namespace ca::num {

template<typename T>
void runge_kutta<T>::check() {
	if (not m_nodes)
		throw std::runtime_error("ca::nm::runge_kutta: no nodes");
	if (not m_f)
		throw std::runtime_error("ca::nm::runge_kutta: no f(t, u(t))");
}

template<typename T>
void runge_kutta<T>::solve() {
	m_y.resize(m_nodes->n());
	m_y[0] = m_u0;
	for (std::size_t i = 1; i < m_nodes->n(); i++) {
		const value_type tau = m_nodes->weight(i-1);
		const value_type tn = m_nodes->nodes()[i-1];
		const value_type k1 = tau*m_f(tn, m_y[i-1]);
		const value_type k2 = tau*m_f(tn + 0.4*tau, m_y[i-1] + 0.4*tau*k1);
		const value_type k3 = tau*m_f(tn + 1*tau, m_y[i-1] - 7*tau*k1/8 + 1.875*tau*k2);
		m_y[i] = m_y[i-1] + k1/12 + 25*k2/36 + 2*k3/9;
	}
}

}
