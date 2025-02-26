#ifndef _CALGO_NUMERIC_RUNGE_KUTTA_HPP_
#define _CALGO_NUMERIC_RUNGE_KUTTA_HPP_

#include <calgo/in/interpolation.hpp>
#include <calgo/vec.hpp>

#include <functional>

namespace ca::num { 

// TODO: joint the euler_cauchy with runge_kutta with same base

/**
 * @brief Runge-Kutta method for Cauchy poblem (3th order)
 *
 * Butcher tableau is given in form:
 * \f[
 * \alpha_1 = 0,\ \alpha_2 = \frac{2}{5},\ \alpha_3 = 1,\\
 * \beta_{21} = \frac{2}{5},\ \beta_{31} = -\frac{7}{8},\ \beta_{32} = 1\frac{7}{8}, \beta_{33} = 0,\\
 * p_1 = \frac{1}{12},\ p_2 = \frac{25}{36},\ p_3 = \frac{2}{9}.
 * \f]
 *
 * @copydetails ca::num::euler_cauchy
 */
template<typename T>
class runge_kutta {
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

#include "../../../src/runge_kutta.inl"

#endif // !_CALGO_NUMERIC_RUNGE_KUTTA_HPP_
