#ifndef _CALGO_NUMERIC_EULER_CAUCHY_HPP_
#define _CALGO_NUMERIC_EULER_CAUCHY_HPP_

#include <calgo/in/interpolation.hpp>
#include <calgo/vec.hpp>

#include <functional>

namespace ca::num {

/**
 * @brief Euler-Cauchy method for Cauchy poblem
 *
 * Problem is given in form 
 * \f[ \frac{\mathrm d u}{\mathrm d t}=f(t,u(t)),\quad u(t_0) = u_0,\quad t_0\leq t \leq t_1.\f]
 *
 * @tparam T `value_type`
 */
template<typename T>
class euler_cauchy {
public:
	using value_type = T;
	using nodes_type = in::Nodes<value_type>;
	using F = std::function<value_type(value_type t, value_type u)>;

public:

	/**
	 * @brief Set nodes
	 * @param nodes input nodes
	 */
	void set_nodes(nodes_type* nodes) { m_nodes = nodes; }
	const nodes_type* nodes() { return m_nodes; }

	/**
	 * @brief Function values
	 * @return function values
	 */
	const vec<value_type>& y() { return m_y; }

	/**
	 * @brief Set \f(f(t,u(t))\f) functor
	 * @param f the new \f(f(t,u(t))\f)
	 */
	void set_f(F f) { m_f = f; }

	/**
	 * @brief Get \f(f(t,u(t))\f) functor
	 * @return f the \f(f(t,u(t))\f)
	 */
	const F& f() { return m_f; }

	/**
	 * @brief Solve the problem
	 */
	virtual void solve();
	/**
	 * @brief Check if all is alright by calling check() and then solve the problem
	 * @throw std::runtime_error
	 */
	void solve_safe() { check(); solve(); };

	/**
	 * @brief Performs pre-solve checking if requested
	 */
	virtual void check();

	/**
	 * @brief Set \f(u(t_0)\f)
	 *
	 * @param u0 \f(u(t_0)\f) value
	 */
	void set_u0(const value_type& u0) {
		m_u0 = u0;
	};
	const value_type& u0() { return m_u0; };

protected:
	nodes_type* m_nodes; /// solution range nodes
	vec<value_type> m_y;
	F m_f;
	value_type m_u0 = 0;
};

}

#include "../../../src/euler_cauchy.inl"

#endif // !_CALGO_NUMERIC_EULER_CAUCHY_HPP_
