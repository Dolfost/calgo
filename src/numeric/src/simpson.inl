#pragma once
#include "calgo/num/simpson.hpp"

#include <stdexcept>

namespace ca::num {

template<typename T>
void simpson<T>::check() {
	if (not m_f)
		throw std::logic_error("ca::num::simpson: f(x) is undefined");
	if (not m_nodes)
		throw std::logic_error("ca::num::simpson: nodes is not defined");
}

template<typename T>
typename simpson<T>::value_type simpson<T>::integrate() {
	const std::size_t& n = m_nodes->n();

	value_type odd = 0.0;
	for (std::size_t i = 1; i < n; i += 2)
		odd += m_f(m_nodes->nodes()[i]);

	value_type even = 0.0;
	for (std::size_t i = 2; i < n; i += 2)
		even += m_f(m_nodes->nodes()[i]);

	return (s_y[0] + s_y[n-1] + 2 * even + 4 * odd) * m_nodes->weight(0) / 3;
}

}
