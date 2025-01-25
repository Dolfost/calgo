#pragma once
#include <calgo/num/boundingPhase.hpp>

namespace ca::num {

template<typename T>
void BoundingPhase<T>::check() {
	if (not m_func)
		throw std::runtime_error("ca::num::BoundingPhase: no function");
	if (m_step == 0)
		throw std::runtime_error("ca::num::BoundingPhase: zero step");
}

template<typename T>
bool BoundingPhase<T>::localize() {
	m_found = false;

	value_type x0 = m_startingPoint, x1 = x0 + m_step, x2;
	value_type f0 = m_func(x0), f1 = m_func(x1), f2;
	value_type direction;

	if (f1 < f0)
		direction = 1;
	else {
		x1 = x0 - m_step;
		direction = -1;
	}

	const bool call = (bool)p_iterationCallback;
	unsigned long long mult = 1;
	size_type i;
	for (i = 0; i < m_iterationLimit; i++) { 
		mult = mult << 1;
		x2 = x1 + direction*mult*m_step;
		f2 = m_func(x2);
		
		if (f0 > f1 and f2 >= f1) {
			m_found = true;
			if (direction == 1)
				m_int = {x0, x2};
			else 
				m_int = {x2, x0};
			break;
		}

		x0 = x1, x1 = x2;
		f0 = f1, f1 = f2;

		if (call)
			p_iterationCallback(x0, x1, x2, f0, f1, f2, i);
	}
	if (call)
		p_iterationCallback(x0, x1, x2, f0, f1, f2, i);

	return m_found;
}

}
