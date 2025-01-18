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

template<typename T, typename I = T>
class NystromFredholmSecondKind {
	static_assert(std::is_arithmetic<I>(), "Not an arithmetic type");
public:
	using value_type = T;
	using interval_type = I;
	using size_type = typename Vec<T>::size_type;
	using nodes_type = in::Nodes<value_type, interval_type>;

	using K = std::function<value_type(value_type, value_type)>;
	using F = std::function<value_type(value_type)>;

public:
	virtual void solve();
	void solve_safe() { check(); solve(); };

	value_type phi(value_type x);
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
	value_type operator()(value_type x) {
			return phi(x);
	}

public:
	void setF(F f) { m_f = f; };
	void setK(K k) { m_k = k; };
	const F& f() { return m_f; };
	const K& k() { return m_k; };

	void setNodes(nodes_type* nodes) { m_nodes = nodes; }
	const nodes_type* nodes() { return m_nodes; }

protected:
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
