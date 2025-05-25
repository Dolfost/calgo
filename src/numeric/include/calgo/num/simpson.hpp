#ifndef _CALGO_NUM_SIMPSON_HPP_
#define _CALGO_NUM_SIMPSON_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>
#include <calgo/in/interpolation.hpp>

#include <cstddef>
#include <functional>

namespace ca::num {

/**
 * @brief Simpson integration formula
 *
 * @tparam T value_type
 */
template<typename T>
class simpson {
public:
	using value_type = T;
	using size_type = std::size_t;
	using nodes_type = ca::in::UniformNodes<value_type>;
	using F_type = std::function<value_type(value_type)>;

public:
	void set_nodes(nodes_type* n) {
		m_nodes = n;
	};
	nodes_type* nodes() { return m_nodes; }

	void set_f(const F_type& f) { m_f = f; }
	const F_type& f() { return f; }

	void check();
	value_type integrate();
	value_type integrate_safe() { check(); return integrate(); }

	value_type operator()() { return integrate(); };

protected:
	ca::vec<value_type> s_y, s_x;
	ca::in::Nodes<value_type>* m_nodes;
	F_type m_f;
};

}

#include "../../../src/simpson.inl"

#endif // !_CALGO_NUM_SIMPSON_HPP_
