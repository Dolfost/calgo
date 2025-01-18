#ifndef _CALGO_LIN_GAUSS_HPP_
#define _CALGO_LIN_GAUSS_HPP_

#include <calgo/lin/system.hpp>
#include <calgo/macros.hpp>

namespace ca::lin {

template<typename T>
class Gauss: public System<T> {
public:
	using typename System<T>::value_type;
	Gauss(
		MatView<value_type> vars = MatView<T>(nullptr, 0, 0), 
		VecView<value_type> constr = VecView<T>(nullptr, 0)
	): m_variables(vars), m_constraints(constr) {}

	void setConstraints(VecView<value_type>& c) { m_constraints = c; };
	void setVariables(MatView<value_type>& v) { m_variables = v; };
	VecView<value_type>& constraints() { return m_variables; };
	MatView<value_type>& variables() { return m_variables; };
	const VecView<value_type>& constraints() const { return m_variables; };
	const MatView<value_type>& variables() const { return m_variables; };

	void init() override;
	void solve() override;

	CA_CALLBACK(
		iteration, 
		const MatView<T>& vars,
		const VecView<T>& constr,
		typename Vec<T>::size_type i
	)
	CA_CALLBACK(
		swap,
		const MatView<T>& vars,
		const VecView<T>& constr,
		typename Vec<T>::size_type row,
		typename Vec<T>::size_type i
	)
	CA_CALLBACK(
		out,
		const MatView<T>& vars,
		const VecView<T>& constr,
		const Vec<T>& sol,
		typename Vec<T>::size_type i
	)
	CA_CALLBACK(
		init,
		const MatView<T>& vars,
		const VecView<T>& constr
	)

protected:
	void check() override;

protected:
		MatView<value_type> m_variables;
		VecView<value_type> m_constraints;
};

}

#include <calgo/nomacros.hpp>
#include "../../../src/gauss.inl"

#endif // !_CALGO_LIN_GAUSS_HPP_
