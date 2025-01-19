#pragma once
#include<calgo/opt/transportationSimplex.hpp>

#include <stdexcept>
#include <utility>

namespace ca::opt {

#define CALL_STRUCT {m_cost, m_distribution, m_demand, m_supply, m_u, m_v, m_basisCells, m_f} 

template<typename T>
void TransportationSimplex<T>::init() {
	m_u.resize(m_cost.rows());
	m_v.resize(m_cost.cols());
	m_distribution.resize(
		m_cost.rows(),
		m_cost.cols()
	);
	m_basisCells.resize(m_cost.rows()+m_cost.cols()-1);

	m_distribution = northWest(m_cost, m_demand, m_supply, m_basisCells);
}

template<typename T>
void TransportationSimplex<T>::optimize() {
	init();

	if (p_iterationCallback) {
		size_type it = 0;
		while (iterate())
			p_iterationCallback(CALL_STRUCT, it++);
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
	static size_type i = 1;
	return i--;
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
ca::Mat<typename TransportationSimplex<T>::value_type> 
TransportationSimplex<T>::northWest(
	ca::Mat<value_type>& cost,
	ca::Vec<value_type>& demand,
	ca::Vec<value_type>& supply,
	cells_type& basisCells
) {
	ca::Mat<value_type> distribution = addSlack(cost, demand, supply);

	size_type i = 0, j = 0, b = 0;
	value_type used = 0, supplied = 0;
	while (i < distribution.rows() and j < distribution.cols()) {
		distribution(i, j) = std::min(supply[i] - used, demand[j] - supplied);
		used += distribution(i, j);
		supplied += distribution(i, j);

		const value_type suppliesLeft = supply[i] - used;
		if (suppliesLeft > 0) {
			if (supplied == demand[j]) {
				j++;
				supplied = 0;
			} else {
				throw std::runtime_error(
					"ca::TransportationSimplex::northWest: "
					"supplied more than demanded (rem!=0)"
				);
			}
		} else if (suppliesLeft == 0) {
			if (supplied < demand[j]) {
				i++;
				used = 0;
			} else if (supplied == demand[j]) {
				basisCells[b++] = {++i, j++};
				used = supplied = distribution(i, j-1) = 0;
			} else {
				throw std::runtime_error(
					"ca::TransportationSimplex::northWest: "
					"supplied more than demanded (rem=0)"
				);
			}
		} else
			throw std::runtime_error(
				"ca::TransportationSimplex::northWest: "
				"negative remainder"
			);

		basisCells[b++] = {i, j};
	}

	return std::move(distribution);
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const TransportationSimplex<D>& s) {
	s.m_cost.showSystem(s.m_supply);
	return os << s.m_demand;
}

#undef CALL_STRUCT

}
