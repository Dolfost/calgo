#pragma once
#include <calgo/num/boundary_value_problem.hpp>

#include <calgo/lin/thomas.hpp>

#include <stdexcept>

namespace ca::num {

template<typename T>
void bvp<T>::check() {
	if (not m_nodes)
		throw std::runtime_error("ca::nm::bvp: no nodes");
	if (not m_f)
		throw std::runtime_error("ca::nm::bvp: no f(x))");
}

template<typename T>
void bvp_undefined_coefficients<T>::check() {
	bvp<T>::check();
	if (not this->m_k)
		throw std::runtime_error("ca::nm::bvp_undefined_coefficients: no k(x)");
	if (not this->m_q)
		throw std::runtime_error("ca::nm::bvp: no q(x)");
}

template<typename T>
void bvp_finite_differences<T>::check() {
	bvp<T>::check();
	if (this->m_k)
		throw std::runtime_error("ca::nm::bvp_finite_differences: k(x) should not be provided");
	if (not this->m_q)
		throw std::runtime_error("ca::nm::bvp: no q(x)");
}

template<typename T>
void bvp_undefined_coefficients<T>::solve() {
	const auto n = this->m_nodes->n()-1; // intervals count
	vec<typename bvp_traits<T>::value_type> b(n-1), f(n-1);
	ca::lin::thomas<typename bvp_traits<T>::value_type> thomas;

	vec<typename bvp_traits<T>::value_type> a(n-2), c(n-2);

	{
		typename vec<typename bvp_traits<T>::value_type>::size_type i;
		for (i = 0; i < n-2; i++) {
			a[i] = this->m_k(this->m_nodes->nodes()[i+2] - this->m_nodes->weight(i+2)/2)/this->m_nodes->weight(i+2);
			b[i] = -(this->m_k(this->m_nodes->nodes()[i+1]+this->m_nodes->weight(i+1)/2) +
				this->m_k(this->m_nodes->nodes()[i+1]-this->m_nodes->weight(i+1)/2))/this->m_nodes->weight(i+1) -
				this->m_q(this->m_nodes->nodes()[i+1])*this->m_nodes->weight(i+1);
			c[i] = this->m_k(this->m_nodes->nodes()[i+1] + this->m_nodes->weight(i+1)/2)/this->m_nodes->weight(i+1);
			f[i] = -this->m_nodes->weight(i+1)*this->m_f(this->m_nodes->nodes()[i+1]);
		}
		b[i] = -(this->m_k(this->m_nodes->nodes()[i+1]+this->m_nodes->weight(i+1)/2) +
			this->m_k(this->m_nodes->nodes()[i+1]-this->m_nodes->weight(i+1)/2))/this->m_nodes->weight(i+1) -
			this->m_q(this->m_nodes->nodes()[i+1])*this->m_nodes->weight(i+1);
		f[i] = (-this->m_nodes->weight(i+1)*this->m_f(this->m_nodes->nodes()[i+1]) - 
			this->m_k(this->m_nodes->nodes()[i+1] + this->m_nodes->weight(i+1)/2)/this->m_nodes->weight(i+1))*this->m_mu.second;
		f[0] -= (this->m_k(this->m_nodes->nodes()[1] - this->m_nodes->weight(1)/2)/this->m_nodes->weight(1))*this->m_mu.first;
	}

	thomas.set_a(a); 
	thomas.set_b(b);
	thomas.set_c(c);
	thomas.set_constraints(f);
	thomas.solve();

	this->m_y.resize(n+1);
	this->m_y[0] = this->m_mu.first;
	this->m_y[n] = this->m_mu.second;
	for (typename vec<typename bvp_traits<T>::value_type>::size_type i = 1; i < n; i++)
		this->m_y[i] = thomas.solution()[i-1];
}

template<typename T>
void ca::num::bvp_finite_differences<T>::solve() {
	const auto n = this->m_nodes->n()-1; // intervals count
	vec<typename bvp_traits<T>::value_type> b(n-1), f(n-1);
	ca::lin::thomas<typename bvp_traits<T>::value_type> thomas;

	vec<typename bvp_traits<T>::value_type> a(n-2), c(n-2);

	vec<typename bvp_traits<T>::value_type> ones(n-2);
	ones.set(1);

	for (typename vec<typename bvp_traits<T>::value_type>::size_type i = 0; i < n-1; i++) {
		auto h_squared = std::pow(this->m_nodes->weight(i+1), 2);
		b[i] = -(2 + h_squared*this->m_q(this->m_nodes->nodes()[i+1]));
		f[i] = -this->m_f(this->m_nodes->nodes()[i+1])*h_squared;
	}
	f[0] -= this->m_mu.first;
	f[n-2] -= this->m_mu.second;

	thomas.set_a(ones); 
	thomas.set_b(b);
	thomas.set_c(ones);
	thomas.set_constraints(f);
	thomas.solve();

	this->m_y.resize(n+1);
	this->m_y[0] = this->m_mu.first;
	this->m_y[n] = this->m_mu.second;
	for (typename vec<typename bvp_traits<T>::value_type>::size_type i = 1; i < n; i++)
		this->m_y[i] = thomas.solution()[i-1];
}

template<typename T>
void bvp_galerkin<T>::check() {
	bvp<T>::check();
	if (not this->m_k)
		throw std::runtime_error("ca::nm::bvp_galerkin: no k(x)");
	if (not dynamic_cast<ca::in::UniformNodes<typename bvp_traits<T>::value_type>*>(this->m_nodes))
		throw std::runtime_error("ca::nm::bvp_galerkin: nodes should be uniform");
	if (this->m_nodes->a() != 0 or this->m_nodes->b() != 1)
		throw std::runtime_error("ca::nm::bvp_galerkin: nodes.a: nodes should be in [0, 1]");
	if (this->m_q)
		throw std::runtime_error("ca::nm::bvp_galerkin: q(x) should not be provided");
}

template<typename T>
typename bvp_traits<T>::value_type bvp_galerkin<T>::m_alpha(
	typename bvp_traits<T>::size_type i,
	typename bvp_traits<T>::size_type j
) {
	auto h = this->m_nodes->weight(0);
	auto h_2 = h/2;
	auto x = this->m_nodes->nodes()[i];
	return (this->m_k(x - h_2)*(m_d(i, j) - m_d(i, j-1)) - this->m_k(x+h_2)*(m_d(i, j+1) - m_d(i, j)))/h;
}

template<typename T>
typename bvp_traits<T>::value_type bvp_galerkin<T>::m_beta(
	typename bvp_traits<T>::size_type i
) {
	auto h = this->m_nodes->weight(0);
	auto h_2 = h/2;
	auto x = this->m_nodes->nodes()[i];
	return h*(this->m_f(x - h_2) + this->m_f(x + h_2))/2;
}

template<typename T>
void ca::num::bvp_galerkin<T>::solve() {
	const auto n = this->m_nodes->n()-1; // intervals count
	vec<typename bvp_traits<T>::value_type> b(n-1), f(n-1);
	vec<typename bvp_traits<T>::value_type> a(n-2), c(n-2);
	ca::lin::thomas<typename bvp_traits<T>::value_type> thomas;
	this->m_mu = {0, 0};

	{
		typename vec<typename bvp_traits<T>::value_type>::size_type i;
		for (i = 0; i < n-2; i++) {
			a[i] = m_alpha(i + 1, i);
			b[i] = m_alpha(i , i);
			c[i] = m_alpha(i, i + 1);
			f[i] = m_beta(i);
		}
		b[i] = m_alpha(i , i);
		f[i] = m_beta(i);
	}

	thomas.set_a(a); 
	thomas.set_b(b);
	thomas.set_c(c);
	thomas.set_constraints(f);
	thomas.solve();

	this->m_y.resize(n+1);
	this->m_y[0] = this->m_mu.first;
	this->m_y[n] = this->m_mu.second;
	for (typename vec<typename bvp_traits<T>::value_type>::size_type i = 1; i < n; i++)
		this->m_y[i] = thomas.solution()[i-1];
}

}
