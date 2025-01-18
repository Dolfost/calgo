#pragma once
#include <calgo/num/nystromFredholm.hpp>

namespace ca::num {

template<typename T, typename I>
typename NystromFredholmSecondKind<T, I>::value_type 
NystromFredholmSecondKind<T, I>::phi(
	typename NystromFredholmSecondKind<T, I>::value_type x
) {
	value_type res = m_f(x);
	for(size_type i = 0; i < m_nodes->n(); i++)
		res += m_nodes->weight(i) * m_k(x, m_nodes->nodes()[i]) * m_y[i];

	return res;
}

template<typename T, typename I>
void NystromFredholmSecondKind<T, I>::solve() {
	Mat<value_type> A(m_nodes->n(), m_nodes->n());
	Vec<value_type> b(m_nodes->n());

	// fill SOLE
	for(size_type j = 0; j < m_nodes->n(); j++) {
		for(size_type k = 0; k < m_nodes->n(); k++)
			A(j, k) = 
				m_nodes->weight(k) * m_k(m_nodes->nodes()[j], m_nodes->nodes()[k]) 
				- (j == k ? 1 : 0);  
		b[j] = -m_f(m_nodes->nodes()[j]);       
	}

	ca::lin::Gauss<value_type> gauss(A, b);
	gauss.solve();
	m_y = gauss.solution();
}

template<typename T, typename I>
void NystromFredholmSecondKind<T, I>::check() {
	if (not m_nodes)
		throw std::runtime_error("ca::Nystrom: no nodes");
	if (not m_f)
		throw std::runtime_error("ca::Nystrom: f(x) not defined");
	if (not m_k)
		throw std::runtime_error("ca::Nystrom: k(a, b) not defined");
	if (m_nodes->b() < m_nodes->a())
		throw std::runtime_error("ca::Nystrom: b is less than a");
	if (m_nodes->n() < 2)
		throw std::runtime_error("ca::Nystrom: n is less than 2");
}

}
