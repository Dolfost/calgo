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

/**
 * @brief Transportation Simplex 
 *
 * Solves tranportation problem using streamlined simplex method
 *
 * @tparam T `value_type`
 */
template<typename T>
class TransportationSimplex {
public:
	struct Cell;
	using value_type = typename ca::Vec<T>::value_type; ///< Value type
	using size_type = typename ca::Vec<T>::size_type; ///< Size type
	using cells_type = std::vector<Cell>; ///< Cell index list type

public:
	/**
	 * @brief Set demand
	 */
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::Vec<value_type>, P>::value>::type setDemand(P && demand) {
		m_demand = std::forward<P>(demand);
	}
	/**
	 * @brief Set supply
	 */
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::Vec<value_type>, P>::value>::type setSupply(P && supply) {
		m_supply = std::forward<P>(supply);
	}
	/**
	 * @brief Set costs
	 */
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::Mat<value_type>, P>::value>::type setCost(P && cost) {
		m_cost = std::forward<P>(cost);
	}

	/**
	 * @brief Set maximization or minimization
	 *
	 * @param m `true` if want to maximize
	 */
	void setMaximize(bool m) { m_maximize = m; }
	bool maximize() { return m_maximize; }


	/**
	 * @brief Get demand
	 *
	 * @return current demand
	 */
	ca::Vec<value_type>& demand() { return m_demand; };
	const ca::Vec<value_type>& demand() const { return m_demand; };
	/**
	 * @brief Get supply
	 *
	 * @return current supply
	 */
	ca::Vec<value_type>& supply() { return m_supply; };
	const ca::Vec<value_type>& supply() const { return m_supply; };
	/**
	 * @brief Get cost 
	 *
	 * @return current cost
	 */
	ca::Mat<value_type>& cost() { return m_cost; };
	const ca::Mat<value_type>& cost() const { return m_cost; };

	/**
	 * @brief Get objective function value
	 *
	 * @return \f(f(x)\f) for current solution
	 */
	const value_type& f() { return m_f; };

	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const TransportationSimplex<D>& s);

	/**
	 * @struct CallbackContext
	 * @brief Contains callback callback context
	 */
	struct CallbackContext {
		const ca::Mat<value_type>& cost, distribution;
		const ca::Vec<value_type>& demand, supply, u, v;
		const cells_type& basisCells;
		const value_type& f; ///< function value
	};

	/**
	 * @struct Cell
	 * @brief contains cell index
	 */
	struct Cell {
		size_type i = 0;
		size_type j = 0;
		bool operator==(const Cell& other) const {
			return i == other.i and j == other.j;
		}
		friend std::ostream& operator<<(std::ostream& os, const Cell& c) {
			return os << '(' << c.i << ',' << c.j << ')';
		}
	};

	enum class BFS {
		Northwest,
		// Vogel, //  TODO: implemet Vogel and Russel
		// Russel
	};

	/**
	 * @brief Check if problem is not mallformed
	 * @throws std::runtime_error
	 */
	void check();
	/**
	 * @brief Perfrom optimization
	 */
	void optimize();
	/**
	 * @brief Perfrom optimize but first call check()
	 */
	void optimize_safe() { check(); optimize(); };

	/**
	 * @brief Initialization callback
	 * Gets called after initial values are set up, and problem is weighted
	 * properly with slack variables
	 */
	CA_CALLBACK(
		init,
		CallbackContext
	)
	/**
	 * @brief Iteration callback
	 * Gets called after every iteration is complete
	 */
	CA_CALLBACK(
		iteration,
		CallbackContext,
		size_type ///< iteration No
	)
	/**
	 * @brief Cycle callback
	 * Gets called when cycle has been found
	 */
	CA_CALLBACK(
		cycleFound,
		CallbackContext,
		const cells_type&, ///< cycle
		const Cell&, ///< entering variable
		const Cell& ///< leaving variable
	)

	/**
	 * @brief Add slack demand or supply
	 *
	 * @param costs overall costs
	 * @param demand demand array
	 * @param supply supply array
	 */
	static ca::Mat<value_type> addSlack(
		ca::Mat<value_type>& costs,
		ca::Vec<value_type>& demand,
		ca::Vec<value_type>& supply
	);
	/**
	 * @brief Get initial basic feasible solution with Northwest corner method
	 *
	 * @param costs cost matrix
	 * @param demand demand
	 * @param supply supply array
	 * @param basisCells basis cels (will be filled with solution)
	 * @param distribution distribution matrix
	 */
	static void northWest(
		ca::Mat<value_type>& costs,
		ca::Vec<value_type>& demand,
		ca::Vec<value_type>& supply,
		cells_type& basisCells,
		ca::Mat<value_type>& distribution
	);

protected:
	/**
	 * @brief Main iteration function
	 *
	 * @return `true`, if iteration should continue, `false` otherwise
	 */
	bool iterate();

	/**
	 * @brief Get \f(u_i\f) and \f(v_i\f)
	 */
	void calculateUV();
	/**
	 * @brief Check for optimality
	 *
	 * @return `true` if optimal, `false` otherwise
	 */
	bool isOptimal();
	/**
	 * @brief Find entering variable 
	 *
	 * @return entering variable index
	 */
	Cell findEnteringVariable();
	/**
	 * @brief Find cycle 
	 *
	 * This function finds cycle using findCycleRow and findCycleCol recursive
	 * functions
	 *
	 * @param entering entering variable index
	 * @return buit cycle
	 */
	cells_type findCycle(const Cell& entering);
	bool findCycleRow(const Cell& cell, cells_type& cycle);
	bool findCycleCol(const Cell& cell, cells_type& cycle);

	/**
	 * @brief Initialize data
	 */
	void init();

protected:
	bool m_maximize = false;
	ca::Vec<value_type> m_demand; ///< Demand
	ca::Vec<value_type> m_supply; ///< Supply
	ca::Mat<value_type> m_cost; ///< Costs

	value_type m_f; ///< Objective function \f(f\f)value 

	ca::Vec<value_type> m_u, m_v; ///< \f(u_i,v_j\f)
	/**
	 * @brief Solution matrix
	 *
	 * Contains current solution values on the cells that are in m_basisCells,
	 * every non-basic cell contain value of \f(c_{ij}-u_i-v_j\f).
	 */
	ca::Mat<value_type> m_distribution;
	cells_type m_basisCells; ///< Indecies of basic variables

	BFS m_BFSmethod = BFS::Northwest; ///< Current method to find basic feasible solution

	bool (*m_comparator)(const value_type& a, const value_type& b); ///< Comparator function
};

}

#include <calgo/nomacros.hpp>

#include "../../../src/transportationSimplex.inl"

#endif // !_CALGO_OPT_TRANSPORTATION_SIMPLEX_HPP_
