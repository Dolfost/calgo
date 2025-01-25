#pragma once
#include <calgo/num/goldenRatioMinimum.hpp>

#include <calgo/calgo.hpp>

#include <cmath>
#include <utility>

namespace ca::num {

template<typename T>
void GoldenRatioMinimum<T>::find() {
	value_type c, d; 
	size_type i = 0;
	const bool call = (bool)p_iterationCallback;
	while (this->m_int.length() > this->m_eps) {
		d = this->m_int.length() * ca::mconst.invphi;
		c = this->m_int.b - d;
		d = this->m_int.a + d;

		if (this->m_func(c) < this->m_func(d))
			this->m_int.b = d;
		else
			this->m_int.a = c;

		if (call)
			p_iterationCallback(this->m_int, i++);
	}

	this->m_x = this->m_int.middle();
}

}
