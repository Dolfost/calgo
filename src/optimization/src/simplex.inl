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
	s_ctx.bas.resize(s_r, 0);

	if (p_initCallback)
		p_initCallback(s_ctx);
}

template<typename T>
void Simplex<T>::pivot(std::size_t row, std::size_t col) {
	T pn = s_ctx.vars->at(row)[col]; // pivot number
	s_ctx.bas[row] = s_ctx.func->at(col);

	for (auto& e: s_ctx.vars->at(row))
	e /= pn;
	s_ctx.constr->at(row) /= pn;

	for (std::size_t i = 0; i < row; i++) {
		T re = s_ctx.vars->at(i)[col];
		for (std::size_t j = 0; j < s_c; j++)
			s_ctx.vars->at(i)[j] -= re*s_ctx.vars->at(row)[j];
		s_ctx.constr->at(i) -= re*s_ctx.constr->at(row);
	}
	for (std::size_t i = row + 1; i < s_r; i++) {
		T re = s_ctx.vars->at(i)[col];
		for (std::size_t j = 0; j < s_c; j++)
			s_ctx.vars->at(i)[j] -= re*s_ctx.vars->at(row)[j];
		s_ctx.constr->at(i) -= re*s_ctx.constr->at(row);
	}

	for (std::size_t i = 0; i < s_c; i++) {
		s_ctx.netEval[i] = 0;
		for (std::size_t j = 0; j < s_r; j++)
			s_ctx.netEval[i] += s_ctx.bas[j]*s_ctx.vars->at(j)[i];
		s_ctx.netEval[i] = s_ctx.func->at(i) - s_ctx.netEval[i];
	}

	s_ctx.f = s_ctx.bas * *s_ctx.constr; // dot product;

	if (p_pivotCallback)
		p_pivotCallback(s_ctx, row, col);
}

template<typename T>
std::size_t Simplex<T>::pivotRow(std::size_t col) {
	T val = std::numeric_limits<T>::max();
	std::size_t row; // basis variable that leave
	std::size_t rowCount = 0; // count of ties
	for (std::size_t i = 0; i < s_r; i++) {
		if (s_ctx.vars->at(i)[col] <= 0)
			continue;
		T tmp = s_ctx.constr->at(i) / s_ctx.vars->at(i)[col];
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

	pivot(row, col);

	return true;
}

template<typename T>
void Simplex<T>::optimize() {
	if (not s_ctx.vars)
		throw std::runtime_error("simplex: no input variables matrix");
	if (not s_ctx.constr)
		throw std::runtime_error("simplex: no constraints vector");
	if (not s_ctx.func)
		throw std::runtime_error("simplex: no function vector");

	if (p_validCallback)
		p_validCallback(s_ctx);

	init();

	const bool call = bool(p_iterationCallback);
	while (iterate())
		if (call)
			p_iterationCallback(s_ctx);

	for (auto const& x : s_ctx.bas)
	if (x == 0) {
		s_ctx.degenerated = true;
		break;
	}
}

template<typename T>
Simplex<T>::Simplex(
	Matrix<T>* v, 
	Vector<T>* c, 
	const Vector<T>* f
) {
	s_ctx.vars = v;
	s_ctx.constr = c;
	s_ctx.func = f;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Simplex<T>& s) {
	s.variables()->system(*s.constraints(), os);
	return os << std::boolalpha 
	<< "Optimal: " <<  s.optimal()
	<< " | Unbounded: " << s.unbounded()
	<< " | Degenerated: " <<  s.degenerated()
	<< " | " << (s.maximize() ? "Max" : "Min") << ": " << s.f();
}

}
