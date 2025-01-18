#pragma once
#include <calgo/in/interpolation.hpp>

#include <cmath>

namespace ca::in {

template<typename T, typename I>
void UniformNodes<T, I>::calculate() {
	interval_type step = std::abs(this->m_a - this->m_b)/(this->m_n - 1);
	this->m_nodes.resize(this->m_n);
	for (size_type k = 0; k < this->m_n; k++)
		this->m_nodes[k] = this->m_a + k*step;
}

template<typename T, typename I>
void ChebyshevNodes<T, I>::calculate() {
	double c1 = (this->m_a+this->m_b)/2.0;
	double c2 = (this->m_b-this->m_a)/2.0;
	this->m_nodes.resize(this->m_n);
	for (long long k = this->m_n-1; k >= 0; k--)
		this->m_nodes[this->m_n - 1 - k] = 
			c1 + c2*std::cos(((2.0*k+1.0)/(2.0*this->m_n))*mconst.pi);
}

}
