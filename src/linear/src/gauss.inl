#pragma once
#include <calgo/lin/gauss.hpp>

#include <cmath>
#include <utility>

namespace ca::lin {

template<typename T>
void Gauss<T>::init() {
	this->m_solution.resize(m_constraints.n());
	this->m_solution.set(0);
	if (p_initCallback)
		p_initCallback(m_variables, m_constraints);
}

template<typename T>
void Gauss<T>::solve() {
	init();

	typename Mat<T>::size_type n = m_variables.rows();

	const bool iteration = (bool)p_iterationCallback;
	const bool swap = (bool)p_swapCallback;
	const bool out = (bool)p_outCallback;

	for (typename Mat<T>::size_type i = 0; i < n; i++) {
		value_type maxEl = std::abs(m_variables[i][i]);
		typename Mat<T>::size_type maxRow = i;
		for (typename Mat<T>::size_type k = i + 1; k < n; k++)
			if (std::abs(m_variables[k][i]) > maxEl) {
				maxEl = m_variables[k][i];
				maxRow = k;
			}

		// swap rows
		for (typename Mat<T>::size_type k = i; k < n; k++)
			std::swap(m_variables[maxRow][k], m_variables[i][k]);
		std::swap(this->m_constraints[maxRow], this->m_constraints[i]);

		if (swap)
			p_swapCallback(m_variables, m_constraints, maxRow, i);

		// forward move
		for (typename Mat<T>::size_type k = i + 1; k < n; k++) {
			value_type c = -m_variables[k][i]/m_variables[i][i];
			for (typename Mat<T>::size_type j = i; j < n; j++)
				if (i == j)
					m_variables[k][j] = 0;
				else
					m_variables[k][j] += c * m_variables[i][j];
			this->m_constraints[k] += c * this->m_constraints[i];
		}
		if (iteration)
			p_iterationCallback(m_variables, m_constraints, i);
	}

	// reverse move
	for (long long i = n - 1; i >= 0; i--) {
		this->m_solution[i] = m_constraints[i]/m_variables[i][i];
		for (long long k = i - 1; k >= 0; k--)
			m_constraints[k] -= m_variables[k][i] * this->m_solution[i];
		if (out)
			p_outCallback(m_variables, m_constraints, this->m_solution, i);
	}
}

template<typename T>
void Gauss<T>::check() {
	if (m_variables.rows() != m_constraints.n())
		throw std::runtime_error(
			"ca::Gauss: number of constraints "
			"is not equal to number of equations"
		);
	if (m_variables.rows() == 0 or m_constraints.n() == 0)
		throw std::runtime_error(
			"ca::Gauss: system is empty"
		);
};
}
