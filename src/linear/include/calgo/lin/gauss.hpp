#ifndef _CALGO_LIN_GAUSS_HPP_
#define _CALGO_LIN_GAUSS_HPP_

#include <calgo/lin/system.hpp>
#include <calgo/macros.hpp>

namespace ca::lin {

template<typename T>
class Gauss: public system<T> {
public:
	using typename system<T>::value_type;
	Gauss(
		mat_view<value_type> vars = mat_view<T>(nullptr, 0, 0), 
		vec_view<value_type> constr = vec_view<T>(nullptr, 0)
	): m_variables(vars), m_constraints(constr) {}

	void setConstraints(vec_view<value_type>& c) { m_constraints = c; };
	void setVariables(mat_view<value_type>& v) { m_variables = v; };
	vec_view<value_type>& constraints() { return m_constraints; };
	mat_view<value_type>& variables() { return m_variables; };
	const vec_view<value_type>& constraints() const { return m_constraints; };
	const mat_view<value_type>& variables() const { return m_variables; };

	void init() override;
	void solve() override;

	CA_CALLBACK(
		iteration, 
		const mat_view<T>& vars,
		const vec_view<T>& constr,
		typename vec<T>::size_type i
	)
	CA_CALLBACK(
		swap,
		const mat_view<T>& vars,
		const vec_view<T>& constr,
		typename vec<T>::size_type row,
		typename vec<T>::size_type i
	)
	CA_CALLBACK(
		out,
		const mat_view<T>& vars,
		const vec_view<T>& constr,
		const vec<T>& sol,
		typename vec<T>::size_type i
	)
	CA_CALLBACK(
		init,
		const mat_view<T>& vars,
		const vec_view<T>& constr
	)

	void check() override;

protected:
	mat_view<value_type> m_variables;
	vec_view<value_type> m_constraints;
};

}

#include <calgo/nomacros.hpp>
#include "../../../src/gauss.inl"

#endif // !_CALGO_LIN_GAUSS_HPP_
