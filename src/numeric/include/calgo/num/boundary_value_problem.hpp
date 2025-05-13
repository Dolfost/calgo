#ifndef _CALGO_NUM_BOUNDARY_VALUE_PROBLEM_
#define _CALGO_NUM_BOUNDARY_VALUE_PROBLEM_

#include <calgo/in/interpolation.hpp>
#include <calgo/vec.hpp>

#include <functional>
#include <utility>

namespace ca::num {

/**
 * @brief Method of finite differences for BVP
 *
 * Problem is given in form
 * \f[
 * 	u'' - q(x)u = -f(x),\ a<x<b,\\
 * u(a) = \mu_1,\\
 * u(b) = \mu_2,
 * \f]
 */
template<typename T>
class boundary_value_problem {
public:
	using value_type = T;
	using mu_type = std::pair<value_type, value_type>;
	using nodes_type = in::Nodes<value_type>;
	using F = std::function<value_type(value_type x)>;
	using Q = F;

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
	 * @brief Set \f(f(x)\f) functor
	 * @param f the new \f(f(x)\f)
	 */
	void set_f(F f) { m_f = f; }

	/**
	 * @brief Get \f(f(x)\f) functor
	 * @return f the \f(f(x)\f)
	 */
	const F& f() { return m_f; }
	/**
	 * @brief Set \f(q(x)\f) functor
	 * @param f the new \f(q(x)\f)
	 */
	void set_q(Q q) { m_q = q; }
	/**
	 * @brief Get \f(q(x)\f) functor
	 * @return f the \f(q(x)\f)
	 */
	const Q& q() { return m_q; }

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
	void set_mu(const mu_type& mu) {
		m_mu = mu;
	};
	const mu_type& mu() { return m_mu; };

protected:
	mu_type m_mu;
	nodes_type* m_nodes; /// solution range nodes
	vec<value_type> m_y;
	F m_f;
	Q m_q;
	value_type m_u0 = 0;
};

}

#include "../../../src/boundary_value_problem.inl"

#endif // !_CALGO_NUM_BOUNDARY_VALUE_PROBLEM_
