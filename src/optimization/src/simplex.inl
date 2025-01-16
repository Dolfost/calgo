#pragma once
#include<calgo/opt/simplex.hpp>

#include <limits>

namespace ca::opt {

template<typename T>
void Simplex<T>::init() {
	s_r = s_ctx.vars->rows();
	s_c = s_ctx.vars->cols();

	if (s_maximize)
		s_comparator = [](auto a, auto b) {
			return a > b;
		};
	else
		s_comparator = [](auto a, auto b) {
			return a < b;
		};

	s_ctx.f = 0;

	for (std::size_t i = 0; i < s_r; i++) {
		if (s_ctx.constr->at(i) < 0) {
			s_ctx.constr->at(i) *= -1;
			s_ctx.vars->at(i) *= -1;
		}
	}

	s_ctx.unbounded = false;
	s_ctx.optimal = false;
	s_ctx.degenerated = false;
	s_ctx.netEval = *s_ctx.func;
	s_ctx.basis.resize(s_r);
	s_ctx.basis.set(0);
	s_ctx.basisIndexes.resize(s_r);
	for (typename ca::Vec<T>::size_type i = 0; i < s_ctx.basisIndexes.n(); i++)
		s_ctx.basisIndexes[i] = (s_c - s_r) + i;

	if (p_initCallback)
		p_initCallback(s_ctx);
}

template<typename T>
void Simplex<T>::pivot(std::size_t row, std::size_t col) {
	T pn = (*s_ctx.vars)(row, col); // pivot number
	s_ctx.basis[row] = s_ctx.func->at(col);

	for (std::size_t i = 0; i < s_c; i++)
		(*s_ctx.vars)(row, i) /= pn;
	(*s_ctx.constr)[row] /= pn;

	for (std::size_t i = 0; i < row; i++) {
		T re = (*s_ctx.vars)(i, col);
		for (std::size_t j = 0; j < s_c; j++)
			(*s_ctx.vars)(i, j) -= re*(*s_ctx.vars)(row, j);
		(*s_ctx.constr)[i] -= re*(*s_ctx.constr)[row];
	}
	for (std::size_t i = row + 1; i < s_r; i++) {
		T re = (*s_ctx.vars)(i, col);
		for (std::size_t j = 0; j < s_c; j++)
			(*s_ctx.vars)(i, j) -= re*(*s_ctx.vars)(row, j);
		(*s_ctx.constr)[i] -= re*(*s_ctx.constr)[row];
	}

	for (std::size_t i = 0; i < s_c; i++) {
		s_ctx.netEval[i] = 0;
		for (std::size_t j = 0; j < s_r; j++)
			s_ctx.netEval[i] += s_ctx.basis[j]*(*s_ctx.vars)(j, i);
		s_ctx.netEval[i] = (*s_ctx.func)[i] - s_ctx.netEval[i];
	}

	s_ctx.f = s_ctx.basis * (*s_ctx.constr); // dot product;

	if (p_pivotCallback)
		p_pivotCallback(s_ctx, row, col);
}

template<typename T>
std::size_t Simplex<T>::pivotRow(std::size_t col) {
	T val = std::numeric_limits<T>::max();
	std::size_t row; // basis variable that leave
	std::size_t rowCount = 0; // count of ties
	for (std::size_t i = 0; i < s_r; i++) {
		if ((*s_ctx.vars)(i, col) <= 0)
			continue;
		T tmp = (*s_ctx.constr)[i] / (*s_ctx.vars)(i, col);
		if (tmp < val) {
			rowCount = 1;
			val = tmp;
			row = i;
		} else if (tmp == val)
			rowCount++; // tie
	}

	// if more than 1 variable is to leave, 
	// mark the solution as degenerated
	if (rowCount > 1)
		s_ctx.degenerated = true;
		// if no variable leaves basis, 
		// problem is unbounded
	else if (rowCount == 0) {
		s_ctx.unbounded = true;
		row = 0;
	}

	return row;
}

template<typename T>
std::size_t Simplex<T>::pivotCol() {
	T val = 0;
	std::size_t col;
	for (std::size_t i = 0; i < s_c; i++)
		if (s_comparator(s_ctx.netEval[i], val)) {
			val = s_ctx.netEval[i];
			col = i;
		}

	if (val == 0)
		s_ctx.optimal = true;

	return col;
}

template<typename T>
bool Simplex<T>::iterate() {
	std::size_t col = pivotCol();
	if (s_ctx.optimal)
		return false;

	std::size_t row = pivotRow(col);

	if (s_ctx.unbounded)
		return false;

	s_ctx.basisIndexes[row] = col; // save index

	pivot(row, col);

	return true;
}

template<typename T>
void Simplex<T>::optimize() {
	if (not s_ctx.vars)
		throw std::runtime_error("ca::Simplex: no input variables matrix");
	if (not s_ctx.constr)
		throw std::runtime_error("ca::Simplex: no constraints vector");
	if (not s_ctx.func)
		throw std::runtime_error("ca::Simplex: no function vector");

	if (s_ctx.vars->cols() != s_ctx.func->n())
		throw std::runtime_error("ca::Simplex: wrong function dimentions");
	if (s_ctx.vars->rows() != s_ctx.constr->n())
		throw std::runtime_error("ca::Simplex: wrong constraints dimentions");

	if (p_validCallback)
		p_validCallback(s_ctx);

	init();

	const bool call = bool(p_iterationCallback);
	while (iterate())
		if (call)
			p_iterationCallback(s_ctx);

	for (typename Vec<T>::size_type i = 0; i < s_ctx.basis.n(); i++)
		if (s_ctx.basis[i] == 0) {
			s_ctx.degenerated = true;
			break;
		}
}

template<typename T>
Simplex<T>::Simplex(
	Mat<T>* v, 
	Vec<T>* c, 
	const Vec<T>* f
) {
	s_ctx.vars = v;
	s_ctx.constr = c;
	s_ctx.func = f;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const Simplex<D>& s) {
	s.variables()->showSystem(*s.constraints(), os);
	return os << std::boolalpha 
	<< "Basis indexes: " << s.s_ctx.basisIndexes << '\n'
	<< "Optimal: " <<  s.optimal()
	<< " | Unbounded: " << s.unbounded()
	<< " | Degenerated: " <<  s.degenerated()
	<< " | " << (s.maximize() ? "Max" : "Min") << ": " << s.f();
}

}
