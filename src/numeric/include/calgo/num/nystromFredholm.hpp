#ifndef _CALGO_NUM_NYSTROM_HPP_
#define _CALGO_NUM_NYSTROM_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>
#include <calgo/mat.hpp>

#include <calgo/lin/gauss.hpp>
#include <calgo/in/interpolation.hpp>

#include <functional>
#include <type_traits>
#include <stdexcept>

namespace ca::num {

/**
 * @brief Solves Fredholm equation of second kind using Nystrom method
 *
 * \f{equation}
 *	\phi(x)=f(x) + \int\limits_a^b K(x,y)\phi(y)\mathrm dy,
 * \f}
 * - \f(\phi(x)\f) is subject function, get via operator()(value_type) or phi(value_type)
 * - \f(a, b\f) are integrtation limits, deduced from passed nodes (via setNodes(nodes_type*))
 * - \f(K(x, y)\f) is a kernel, and can be set via setK(K)
 * - \f(f(x)\f) is a free unit, and can be set via setF(F)
 *
 * @note For some reason the kernel should be passed as \f(-1\cdot K(x, y)\f)
 *
 * @tparam T `value_type`
 * @tparam I `interval_type`
 */
template<typename T, typename I = T>
class NystromFredholmSecondKind {
	static_assert(std::is_arithmetic<I>(), "Not an arithmetic type");
public:
	using value_type = T;
	using interval_type = I;
	using size_type = typename Vec<T>::size_type;
	using nodes_type = in::Nodes<value_type, interval_type>;

	using K = std::function<value_type(value_type, value_type)>; ///< Kernel type
	using F = std::function<value_type(value_type)>; ///< Free unit type

public:
	/**
	 * @brief Solve the problem
	 */
	virtual void solve();
	/**
	 * @brief Check if all is alright and then solve the problem
	 * @throw std::runtime_error std::out_of_range
	 */
	void solve_safe() { check(); solve(); };

	/**
	 * @brief Get \f(\phi(x)\f)
	 *
	 * @param x argument of \f(\phi\f), if `x` is not in integration range, then
	 * behaivour is undefined
	 * @return \f(\phi(x)\f) value
	 */
	value_type phi(value_type x);
	/**
	 * @copydoc phi(value_type)
	 * @throws std::runtime_error std::out_of_range
	 */
	value_type phi_safe(value_type x) {
		if (not m_f)
			throw std::runtime_error("ca::Nystrom: f(x) not defined");
		if (not m_k)
			throw std::runtime_error("ca::Nystrom: k(a, b) not defined");
		if (not m_nodes)
			throw std::runtime_error("ca::Nystrom: no nodes");
		if (x < m_nodes->a() or x > m_nodes->b())
			throw std::out_of_range("ca::Nystrom: solution argument out of integrand range");
		return phi(x);
	};
	/**
	 * @copydoc phi(value_type)
	 */
	value_type operator()(value_type x) {
		return phi(x);
	}

public:
	/**
	 * @brief Set kernel functor
	 *
	 * @param f kernel functor
	 */
	void setF(F f) { m_f = f; };
	/**
	 * @brief Set free unit functor
	 *
	 * @param k free unit functor
	 */
	void setK(K k) { m_k = k; };
	const F& f() { return m_f; };
	const K& k() { return m_k; };

	/**
	 * @brief Set nodes
	 *
	 * @param nodes input nodes
	 */
	void setNodes(nodes_type* nodes) { m_nodes = nodes; }
	const nodes_type* nodes() { return m_nodes; }

protected:
	/**
	 * @brief Performs pre-solve checking if requested
	 */
	virtual void check();

protected:
	F m_f;
	K m_k;
	Vec<value_type> m_y;
	nodes_type* m_nodes = nullptr;
};

}

#include "../../../src/nystromFredholm.inl"

#endif // !_CALGO_NUM_NYSTROM_HPP_
