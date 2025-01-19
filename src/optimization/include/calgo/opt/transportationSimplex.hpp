#ifndef _CALGO_OPT_TRANSPORTATION_SIMPLEX_HPP_
#define _CALGO_OPT_TRANSPORTATION_SIMPLEX_HPP_

#include <calgo/calgo.hpp>

#include <calgo/vec.hpp>
#include <calgo/mat.hpp>

#include <calgo/macros.hpp>

#include <type_traits>
#include <vector>
#include <utility>

namespace ca::opt {

template<typename T>
class TransportationSimplex {
public:
	struct Cell;
	using value_type = typename ca::Vec<T>::value_type;
	using size_type = typename ca::Vec<T>::size_type;
	using cells_type = std::vector<Cell>;

public:
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::Vec<value_type>, P>::value>::type setDemand(P && demand) {
		m_demand = std::forward<P>(demand);
	}
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::Vec<value_type>, P>::value>::type setSupply(P && supply) {
		m_supply = std::forward<P>(supply);
	}
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::Mat<value_type>, P>::value>::type setCost(P && cost) {
		m_cost = std::forward<P>(cost);
	}
	ca::Vec<value_type>& demand() { return m_demand; };
	const ca::Vec<value_type>& demand() const { return m_demand; };
	ca::Vec<value_type>& supply() { return m_supply; };
	const ca::Vec<value_type>& supply() const { return m_supply; };
	ca::Mat<value_type>& cost() { return m_cost; };
	const ca::Mat<value_type>& cost() const { return m_cost; };

	const value_type& f() { return m_f; };

	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const TransportationSimplex<D>& s);

	struct CallbackContext {
		const ca::Mat<value_type>& cost, distribution;
		const ca::Vec<value_type>& demand, supply, u, v;
		const cells_type& basisCells;
		const value_type& f; ///< function value
	};

	struct Cell {
		size_type i = 0;
		size_type j = 0;
	};

	enum class BFS {
		Northwest,
		VogelApprox,
		RusselApprox
	};

	void check();
	void optimize();
	void optimize_safe() { check(); optimize(); };

	CA_CALLBACK(
		iteration,
		CallbackContext,
		size_type ///< iteration No
	)

	static ca::Mat<value_type> addSlack(
		ca::Mat<value_type>& costs,
		ca::Vec<value_type>& demand,
		ca::Vec<value_type>& supply
	);
	static ca::Mat<value_type> northWest(
		ca::Mat<value_type>& costs,
		ca::Vec<value_type>& demand,
		ca::Vec<value_type>& supply,
		cells_type& basisCells
	);

protected:
	/**
	 * @brief Main iteration function
	 *
	 * @return `true`, if iteration should continue, `false` otherwise
	 */
	bool iterate();

	/**
	 * @brief Find next pivot column
	 *
	 * @return column index
	 */
	std::size_t pivotCol();

	/**
	 * @brief Find next pivot row
	 *
	 * @return row index
	 */
	std::size_t pivotRow(std::size_t col);

	/**
	 * @brief Perform pivoting
	 *
	 * @param row pivot row
	 * @param col pivot col
	 */
	void pivot(std::size_t row, std::size_t col);

	/**
	 * @brief Initialize simplex tableau
	 */
	void init();

protected:
	bool m_maximize = false;
	ca::Vec<value_type> m_demand;
	ca::Vec<value_type> m_supply;
	ca::Mat<value_type> m_cost;
	value_type m_f;

	ca::Vec<value_type> m_u, m_v;
	ca::Mat<value_type> m_distribution;
	cells_type m_basisCells;

	BFS m_BFSmethod = BFS::Northwest;

	bool (*m_comparator)(const value_type& a, const value_type& b);
};

}

#include <calgo/nomacros.hpp>

#include "../../../src/transportationSimplex.inl"

#endif // !_CALGO_OPT_TRANSPORTATION_SIMPLEX_HPP_
