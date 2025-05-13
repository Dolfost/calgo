#ifndef _CALGO_LIN_SYSTEM_HPP_
#define _CALGO_LIN_SYSTEM_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>
#include <calgo/mat.hpp>

#include <stdexcept>

namespace ca::lin {

template<typename T>
class System {
public:
	using value_type = T;

	virtual void init() = 0;
	virtual void solve() = 0;
	void solve_safe() { check(); solve(); };

	vec<value_type>& solution() { return m_solution; };

	virtual void check() = 0;

protected:
	vec<value_type> m_solution;
};

}

#endif // !_CALGO_LIN_SYSTEM_HPP_
