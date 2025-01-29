#pragma once
#include <calgo/num/fibonacciMinimum.hpp>

#include <cmath>
#include <utility>

namespace ca::num {

template<typename T> 
typename FibonacciMinimum<T>::value_type FibonacciMinimum<T>::fib(value_type n) {
	if (n == 0 or n == 1)
		return 1;
	return FibonacciMinimum<T>::fib(n-1)+ FibonacciMinimum<T>::fib(n-2);
}

template<typename T>
void FibonacciMinimum<T>::find() {
	value_type fibIt = fib(m_it);
	value_type d = (fib(m_it - 1) / fibIt)*this->m_int.length() + ((m_it % 2 ? -1 : 1)/fibIt)*this->m_eps;

	value_type x1 = this->m_int.b - d, x2 = this->m_int.a + d;
	const bool call = (bool)p_iterationCallback;

	for (size_type i = 0; i < m_it; i++) {
		if (this->m_func(x1) < this->m_func(x2)) {
			this->m_int.b = x2;
			x2 = this->m_int.b - std::abs(x1 - this->m_int.a);
		} else {
			this->m_int.a = x1;
			x1 = this->m_int.a + std::abs(this->m_int.b - x2);
		}

		if (x1 > x2)
			std::swap(x1, x2);
		if (call)
			p_iterationCallback(this->m_int, i);
	}

	this->m_x = this->m_int.middle();
}

}
