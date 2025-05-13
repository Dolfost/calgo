#ifndef _CALGO_LSYS_THOMAS_HPP_
#define _CALGO_LSYS_THOMAS_HPP_

#include <calgo/calgo.hpp>

#include <calgo/lin/system.hpp>

namespace ca::lin {

template<typename T>
class Thomas: public System<T> {
public:
	using typename System<T>::value_type;
public:
	void init() override;
	void check() override;
	void solve() override;

	void set_constraints(vec_view<value_type>& c) { m_constraints = c; };
	void set_a(vec_view<value_type>& a) { m_a = a; };
	void set_b(vec_view<value_type>& b) { m_b = b; };
	void set_c(vec_view<value_type>& c) { m_c = c; };
	vec_view<value_type>& constraints() { return m_constraints; };
	const vec_view<value_type>& a() const { return a; };
	const vec_view<value_type>& b() const { return b; };
	const vec_view<value_type>& c() const { return c; };
	vec_view<value_type>& a() { return a; };
	vec_view<value_type>& b() { return b; };
	vec_view<value_type>& c() { return c; };
	const vec_view<value_type>& constraints() const { return m_constraints; };

protected:
	vec_view<value_type> m_a, m_b, m_c;
	vec_view<value_type> m_constraints;
};

}

#include "../../../src/thomas.inl"

#endif // !_CALGO_LSYS_THOMAS_HPP_
