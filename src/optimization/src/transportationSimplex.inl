#pragma once
#include<calgo/opt/transportationSimplex.hpp>

#include <stdexcept>
#include <utility>
#include <cassert>
#include <list>
#include <algorithm>
#include <limits>

namespace ca::opt {

#define CALL_STRUCT {m_cost, m_distribution, m_demand, m_supply, m_u, m_v, m_basisCells, m_f} 

template<typename T>
void TransportationSimplex<T>::init() {
	if (m_maximize)
		m_comparator = [](auto a, auto b) { return a <= b; };
	else
		m_comparator = [](auto a, auto b) { return a >= b; };

	m_distribution = addSlack(m_cost, m_demand, m_supply);

	m_basisCells.resize(m_cost.rows()+m_cost.cols()-1);
	m_u.resize(m_cost.rows());
	m_v.resize(m_cost.cols());

	northWest(m_cost, m_demand, m_supply, m_basisCells, m_distribution);

	m_f = 0;
	for (const auto& b: m_basisCells) {
		m_f += m_cost(b.i, b.j)*m_distribution(b.i, b.j);
	}

	if (p_initCallback)
		p_initCallback(CALL_STRUCT);
}

template<typename T>
void TransportationSimplex<T>::optimize() {
	init();

	if (p_iterationCallback) {
		size_type it = 0;
		while (iterate())
			p_iterationCallback(CALL_STRUCT, it++);
		p_iterationCallback(CALL_STRUCT, it);
	} else while(iterate());
}

template<typename T>
void TransportationSimplex<T>::check() {
	if (not m_cost.rows()*m_cost.cols())
		throw std::runtime_error("ca::TransportationSimplex: no costs");
	if (m_cost.cols() != m_demand.n())
		throw std::runtime_error("ca::TransportationSimplex: malformed demand");
	if (m_cost.rows() != m_supply.n())
		throw std::runtime_error("ca::TransportationSimplex: malformed supply");
}

template<typename T>
bool TransportationSimplex<T>::iterate() {
	calculateUV();

	if (isOptimal())
		return false;

	Cell entering = findEnteringVariable();
	cells_type cycle = findCycle(entering);

	Cell leaving;
	value_type delta = m_maximize ? std::numeric_limits<double>::min() : std::numeric_limits<double>::max();
	// += 2 bacause only donor cells can share allocation
	for (typename cells_type::size_type i = 1; i < cycle.size(); i += 2) {
		if (not m_comparator(m_distribution(cycle[i].i, cycle[i].j), delta)) {
			delta = m_distribution(cycle[i].i, cycle[i].j);
			leaving = cycle[i];
		}
	}

	if (p_cycleFoundCallback)
		p_cycleFoundCallback(CALL_STRUCT, cycle, entering, leaving);

	if (cycle.size() < 4)
		throw std::runtime_error("ca::TransportationSimplex: cycle not found");

	for (typename cells_type::size_type i = 0; i < cycle.size(); i++)
		m_distribution(cycle[i].i, cycle[i].j) += ((i+1) % 2 ? -delta : delta);

	m_f += delta*(m_cost(entering.i, entering.j) - m_u[entering.i] - m_v[entering.j]);

	assert(std::find(m_basisCells.begin(), m_basisCells.end(), leaving) != m_basisCells.end());
	(*std::find(m_basisCells.begin(), m_basisCells.end(), leaving)) = entering;

	// static size_type i = 1;
	// return i--; //  WARN: only 1 iteration
	return true;
}

template<typename T>
typename TransportationSimplex<T>::cells_type 
TransportationSimplex<T>::findCycle(const Cell& entering) {
	cells_type cycle = {entering};
	findCycleRow(entering, cycle);
	return std::move(cycle);
}

template<typename T>
bool TransportationSimplex<T>::findCycleRow(const Cell& cell, cells_type& cycle) {
	for (const auto& b: m_basisCells) {
		if (b.j != cell.j or b == cell)
			continue;
		if (findCycleCol(b, cycle)) {
			cycle.push_back(b);
			return true;
		}
	}
	return false;
}

template<typename T>
bool TransportationSimplex<T>::findCycleCol(const Cell& cell, cells_type& cycle) {
	if (cell.i == cycle.front().i) // if in same row as entering variable
		return true;
	for (const auto& b: m_basisCells) {
		if (b.i != cell.i or b == cell)
			continue;
		if (findCycleRow(b, cycle)) {
			cycle.push_back(b);
			return true;
		}
	}
	return false;
}

template<typename T>
typename TransportationSimplex<T>::Cell 
TransportationSimplex<T>::findEnteringVariable() {
	Cell entering = {0, 0};
	value_type diff = 0;

	std::list<Cell> basis(m_basisCells.begin(), m_basisCells.end());
	for (size_type i = 0; i < m_distribution.rows(); i++) {
		for (size_type j = 0; j < m_distribution.cols(); j++) {
			auto in = std::find(basis.begin(), basis.end(), Cell{i, j});
			if (in != basis.end()) { // ij is a basic variable
				basis.erase(in); // won't be searched for again
				continue;
			}
			if (not m_comparator(m_distribution(i, j), diff)) {
				diff = m_distribution(i, j);
				entering = {i, j};
			}
		}
	}
	return std::move(entering);
}

template<typename T>
bool TransportationSimplex<T>::isOptimal() {
	std::list<Cell> basis(m_basisCells.begin(), m_basisCells.end());
	bool optimal = true;
	for (size_type i = 0; i < m_distribution.rows(); i++) {
		for (size_type j = 0; j < m_distribution.cols(); j++) {
			auto in = std::find(basis.begin(), basis.end(), Cell{i, j});
			if (in != basis.end()) { // ij is a basic variable
				basis.erase(in); // won't be searched for again
				continue;
			}

			m_distribution(i, j) = m_cost(i, j) - m_u[i] - m_v[j];
			if (not m_comparator(m_distribution(i, j), 0))
				optimal = false;
		}
	}
	return optimal;
}

template<typename T>
void TransportationSimplex<T>::calculateUV() {
	std::vector<bool> u(m_u.n(), false), v(m_v.n(), false);
	m_u[0] = 0; u[0] = true;
	do {
		for (const auto& b: m_basisCells) {
			assert(b.i < u.size()), assert(b.j < v.size());
			if (u[b.i] and not v[b.j]) { // m_u[b.i] is known but m_v[b.j] is not
				m_v[b.j] = m_cost(b.i, b.j) - m_u[b.i];
				v[b.j] = true; // mark as found
			} else if (v[b.j] and not u[b.i]) { // opposite
				m_u[b.i] = m_cost(b.i, b.j) - m_v[b.j];
				u[b.i] = true; // mark as found
			}
		}
	} while ( // still have unknowns
		not std::all_of(u.begin(), u.end(), [](auto i){ return i; }) or
		not std::all_of(v.begin(), v.end(), [](auto i){ return i; })
	);
}

template<typename T>
ca::Mat<typename TransportationSimplex<T>::value_type> 
TransportationSimplex<T>::addSlack(
	ca::Mat<value_type>& cost,
	ca::Vec<value_type>& demand,
	ca::Vec<value_type>& supply
) {
	ca::Mat<value_type> distribution;
	value_type d = demand.sum(), s = supply.sum();
	size_type r = cost.rows(), c = cost.cols();
	if (d < s) {
		c++;
		demand.resize(c, true);
		demand[c-1] = s - d;
	} else if (d > s) {
		r++;
		supply.resize(r, true);
		supply[r-1] = d - s;
	}
	distribution.resize(r, c);
	cost.resize(r, c, true);
	return std::move(distribution);
}

template<typename T>
void TransportationSimplex<T>::northWest(
	ca::Mat<value_type>& cost,
	ca::Vec<value_type>& demand,
	ca::Vec<value_type>& supply,
	cells_type& basisCells,
	ca::Mat<value_type>& distribution
) {
	size_type nextDemand = 0, b = 0;
	value_type supplied = 0, used = 0;

	for (size_type i = 0; i < supply.n(); i++)
		for (size_type j = nextDemand; j < demand.n(); j++) {
			distribution(i, j) = std::min(supply[i] - used, demand[j] - supplied);
			assert(b < basisCells.size());
			basisCells[b++] = {i, j};
			if (distribution(i, j) > 0) {
				supplied += distribution(i, j);
				used += distribution(i, j);
				if (supply[i] - used == 0) {
					used = 0;
					nextDemand = j;
					break;
				}
			}
			supplied = 0;
		}
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const TransportationSimplex<D>& s) {
	s.m_cost.showSystem(s.m_supply);
	return os << s.m_demand;
}

#undef CALL_STRUCT

}
