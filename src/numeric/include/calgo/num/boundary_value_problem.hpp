#ifndef _CALGO_NUM_BOUNDARY_VALUE_PROBLEM_
#define _CALGO_NUM_BOUNDARY_VALUE_PROBLEM_

#include <calgo/in/interpolation.hpp>
#include <calgo/vec.hpp>

#include <functional>
#include <utility>
#include <cstddef>

namespace ca::num {

/**
 * @brief Traits for bvp
 *
 * @tparam T value_type
 */
/**
 * @brief Boundary value problem types
 *
 * @tparam T `value_type`
 */
template <typename T> 
struct bvp_traits {
	using value_type = T;
	using mu_type = std::pair<value_type, value_type>;
	using nodes_type = in::Nodes<value_type>;
	using size_type = std::size_t;
	using F_type = std::function<value_type(value_type x)>;
	using Q_type = F_type;
	using K_type = F_type;
};

/**
 * @brief Boundary value problem base class
 *
 * This class denotes problem in form 
 * \f[
 * 	(k(x)u')' - q(x)u = -f(x),\ a<x<b.
 * \f]
 *
 */
template<typename T>
class bvp {
public:
	/**
	 * @brief Set nodes
	 * @param nodes input nodes
	 */
	void set_nodes(typename bvp_traits<T>::nodes_type* nodes) { m_nodes = nodes; }
	const typename bvp_traits<T>::nodes_type* nodes() { return m_nodes; }

	/**
	 * @brief Function values
	 * @return function values
	 */
	const vec<typename bvp_traits<T>::value_type>& y() { return m_y; }

	/**
	 * @brief Set \f(f(x)\f) functor
	 * @param f the new \f(f(x)\f)
	 */
	void set_f(typename bvp_traits<T>::F_type f) { m_f = f; }

	/**
	 * @brief Get \f(f(x)\f) functor
	 * @return f the \f(f(x)\f)
	 */
	const typename bvp_traits<T>::F_type& f() { return m_f; }
	/**
	 * @brief Set \f(q(x)\f) functor
	 * @param f the new \f(q(x)\f)
	 */
	void set_q(typename bvp_traits<T>::Q_type q) { m_q = q; }
	/**
	 * @brief Get \f(q(x)\f) functor
	 * @return f the \f(q(x)\f)
	 */
	const typename bvp_traits<T>::Q_type& q() { return m_q; }
	/**
	 * @brief Set \f(k(x)\f) functor
	 * @param k the new \f(k(x)\f)
	 */
	void set_k(typename bvp_traits<T>::K_type k) { m_k = k; }
	/**
	 * @brief Get \f(k(x)\f) functor
	 * @return the \f(k(x)\f)
	 */
	const auto& k() { return m_k; }

	/**
	 * @brief Solve the problem
	 */
	virtual void solve() = 0;
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
	 * @brief Set \f(\mu_{1,2}\f)
	 *
	 * @param u0 \f(\mu_{1,2}\f) value
	 */
	void set_mu(const typename bvp_traits<T>::mu_type& mu) {
		m_mu = mu;
	};
	/**
	 * @brief Get \f(\mu_{1,2}\f)
	 *
	 * @return \f(\mu_{1,2}\f) value
	 */
	const typename bvp_traits<T>::mu_type& mu() { return m_mu; };

protected:
	typename bvp_traits<T>::mu_type m_mu;
	typename bvp_traits<T>::nodes_type* m_nodes; /// solution range nodes
	vec<typename bvp_traits<T>::value_type> m_y;
	typename bvp_traits<T>::F_type m_f;
	typename bvp_traits<T>::Q_type m_q;
	typename bvp_traits<T>::K_type m_k;
};

/**
 * @brief Boundary value problem solution using undefined coeficients
 * Problem looks like this:
 * \f[
 * 	(k(x)u')' - q(x)u = -f(x),\ a<x<b.
 * \f]
 * The solve() would be done with method of undefined coefficients.
 *
 * @tparam T `value_type`
 */
template<typename T>
class bvp_undefined_coefficients: public bvp<T> {
public:
	virtual void solve() override;
	virtual void check() override;
};

/**
 * @brief Boundary value problem solution using finite differences
 * Problem is given in form
 * \f[
 * 	u'' - q(x)u = -f(x),\ a<x<b,\\
 * u(a) = \mu_1,\\
 * u(b) = \mu_2,
 * \f]
 * and solved with finite differences method.
 *
 * @tparam T `value_type`
 */
template<typename T>
class bvp_finite_differences: public bvp<T> {
public:
	virtual void solve() override;
	virtual void check() override;
};

/**
 * @brief Boundary value problem solution using Glerkin method
 * Problem is given in form
 * \f[
 * 	(k(x)u')' - q(x)u = -f(x),\ a<x<b,\\
 * u(a) = \mu_1,\\
 * u(b) = \mu_2,
 * \f]
 * and solved with finite differences method. You need to set integral
 * estimator with set_int_estimator() before making some calculations.
 *
 * @tparam T `value_type`
 */
template<typename T>
class bvp_galerkin: public bvp<T> {
public:
	using integral_function = std::function<typename bvp_traits<T>::value_type(typename bvp_traits<T>::value_type)>; /// \f(f(x)\f)
	using integral_estimator = std::function<
	typename bvp_traits<T>::value_type(const integral_function&, typename bvp_traits<T>::value_type, typename bvp_traits<T>::value_type)
	>; /// This function takes some function \f(f(x)\f) and bounds \f(a,b\f) and returns \f(\int_\limits_a^bf(x)\mathrm dx\f)

public:
	virtual void solve() override;
	virtual void check() override;

	/**
	 * @brief Returns \f(\phi_i(x)\f).
	 */
	typename bvp_traits<T>::value_type phi(typename bvp_traits<T>::size_type i, typename bvp_traits<T>::value_type);
	/**
	 * @brief Returns \f(\phi_i'(x)\f).
	 */
	typename bvp_traits<T>::value_type phi_derivative(typename bvp_traits<T>::size_type i,typename bvp_traits<T>::value_type);

	/**
	 * @brief Set the function that will take integrals
	 * This function takes some function \f(f(x)\f) and bounds \f(a,b\f) and
	 * returns \f(\int_\limits_a^bf(x)\mathrm dx\f).
	 * Required by galerkin method.
	 *
	 * You could try to use ca::num::simpson for it. 
	 *
	 * @param i integral estimator functor
	 */
	void set_int_estimator(const integral_estimator& i) {
		m_int_estimator = i;
	}

	/**
	 * @brief Get integral estimator
	 *
	 * @return Current integral estimator
	 */
	const integral_estimator& int_estimator() const { return m_int_estimator; }

protected:
	integral_estimator m_int_estimator;
	typename bvp_traits<T>::value_type m_a(typename bvp_traits<T>::size_type i, typename bvp_traits<T>::size_type j);
};


}

#include "../../../src/boundary_value_problem.inl"

#endif // !_CALGO_NUM_BOUNDARY_VALUE_PROBLEM_
