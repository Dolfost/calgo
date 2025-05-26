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
	if (not m_q)
		throw std::runtime_error("ca::nm::bvp: no q(x)");
}

template<typename T>
void bvp_undefined_coefficients<T>::check() {
	bvp<T>::check();
	if (not this->m_k)
		throw std::runtime_error("ca::nm::bvp_undefined_coefficients: no k(x)");
}
template<typename T>
void bvp_finite_differences<T>::check() {
	bvp<T>::check();
	if (this->m_k)
		throw std::runtime_error("ca::nm::bvp_finite_differences: k(x) should not be provided");
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
	// if (not dynamic_cast<ca::in::UniformNodes<typename bvp_traits<T>::value_type>>(this->m_nodes))
	// throw std::runtime_error("ca::nm::bvp_galerkin: nodes should be uniform");
	if (this->m_q)
		throw std::runtime_error("ca::nm::bvp_galerkin: q(x) should not be provided");
}

template<typename T>
typename bvp_traits<T>::value_type bvp_galerkin<T>::phi(
	typename bvp_traits<T>::size_type i,
	typename bvp_traits<T>::value_type x
) {
	if (i == 0) {
		if (x > this->m_nodes->nodes()[0] and x < this->m_nodes->nodes()[1])
			return -(x -  this->m_nodes->nodes()[1])/this->m_nodes->weight(0);
		else 
			return 0;
	} else if (i == this->m_nodes->n()-2) {
		if (x > this->m_nodes->nodes()[this->m_nodes->n() - 2] and x < this->m_nodes->nodes()[this->m_nodes->n() - 1])
			return (x -  this->m_nodes->nodes()[this->m_nodes->n()-2])/this->m_nodes->weight(this->m_nodes->n()-2);
		else 
			return 0;
	} 

	if (x > this->m_nodes->nodes()[i-1]) {
		if (x < this->m_nodes->nodes()[i])
			return (x - this->m_nodes->nodes()[i-1])/this->m_nodes->weight(i-1);
		else if (x < this->m_nodes->nodes()[i+1])
			return -(x - this->m_nodes->nodes()[i+1])/this->m_nodes->weight(i);
	}

	return 0;
}

template<typename T>
typename bvp_traits<T>::value_type bvp_galerkin<T>::phi_derivative(
	typename bvp_traits<T>::size_type i,
	typename bvp_traits<T>::value_type x
) {
	if (x > this->m_nodes->nodes()[i-1]) {
		if (x < this->m_nodes->nodes()[i])
			return 1/this->m_nodes->weight(i-1);
		else if (x < this->m_nodes->nodes()[i+1])
			return -1/this->m_nodes->weight(i);
	}
	return 0;
}

template<typename T>
void ca::num::bvp_galerkin<T>::solve() {
	const auto n = this->m_nodes->n()-1; // intervals count
	vec<typename bvp_traits<T>::value_type> b(n-1), f(n-1);
	vec<typename bvp_traits<T>::value_type> a(n-2), c(n-2);
	ca::lin::thomas<typename bvp_traits<T>::value_type> thomas;

	std::function<typename bvp_traits<T>::value_type(
		typename bvp_traits<T>::size_type,
		typename bvp_traits<T>::size_type,
		typename bvp_traits<T>::value_type
	)> fx = [this](auto i, auto j, auto x) {
		return this->m_k(x) * phi_derivative(i, x) * phi_derivative(j, x) +
		this->m_q(x) * phi(i, x) * phi(j, x);
	};

	std::function<typename bvp_traits<T>::value_type(
		typename bvp_traits<T>::size_type,
		typename bvp_traits<T>::value_type
	)> fx_b = [this](auto i, auto x) {
		return this->m_f(x) * this->phi(i, x);
	};

	{
		using namespace std::placeholders;
		typename vec<typename bvp_traits<T>::value_type>::size_type i;
		for (i = 0; i < n-2; i++) {
			a[i] = m_int_estimator(std::bind(fx, i+1, i, _1), this->m_nodes->a(), this->m_nodes->b());
			b[i] = m_int_estimator(std::bind(fx, i, i,   _1), this->m_nodes->a(), this->m_nodes->b());
			c[i] = m_int_estimator(std::bind(fx, i, i+1, _1), this->m_nodes->a(), this->m_nodes->b());
			f[i] = m_int_estimator(std::bind(fx_b, i,    _1), this->m_nodes->a(), this->m_nodes->b());
			std::cout << a[i] << " " << b[i] << " " << c[i] << " " << f[i] << std::endl;
		}
		b[i] = m_int_estimator(std::bind(fx, i, i, _1), this->m_nodes->a(), this->m_nodes->b());
		f[i] = m_int_estimator(std::bind(fx_b, i, _1),  this->m_nodes->a(), this->m_nodes->b());
		f[0] -= this->m_mu.first;
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
