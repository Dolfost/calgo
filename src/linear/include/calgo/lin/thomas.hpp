#ifndef _CALGO_LSYS_THOMAS_HPP_
#define _CALGO_LSYS_THOMAS_HPP_

#include <calgo/calgo.hpp>

#include <calgo/lin/system.hpp>

namespace ca::lin {

/**
 * @brief Thomas method
 *
 * Used for tridiagonal systems of linear equations.
 * \f[
 * \begin{bmatrix}
 * b_1 & c_1 & 0 & \cdots & 0 \\
 * a_2 & b_2 & c_2 & \ddots & \vdots \\
 * 0 & a_3 & b_3 & \ddots & 0 \\
 * \vdots & \ddots & \ddots & \ddots & c_{n-1} \\
 * 0 & \cdots & 0 & a_n & b_n
 * \end{bmatrix}
 * \begin{bmatrix}
 * x_1 \\
 * x_2 \\
 * x_3 \\
 * \vdots \\
 * x_n
 * \end{bmatrix}
 * =
 * \begin{bmatrix}
 * d_1 \\
 * d_2 \\
 * d_3 \\
 * \vdots \\
 * d_n
 * \end{bmatrix}
 * \f]
 * where 
 * \f[
 * a_1 = 0,\quad c_n = 0.
 * \f]
 *
 * @tparam T `value_type`
 */
template<typename T>
class thomas: public system<T> {
public:
	using typename system<T>::value_type;
public:
	void init() override;
	void check() override;
	void solve() override;

	/**
	 * @brief The \f(d\f) vector
	 *
	 * @param d the constraints
	 */
	void set_constraints(vec_view<value_type>& d) { m_constraints = d; };
	/**
	 * @brief Set the lower vector
	 *
	 * @param a vector value
	 */
	void set_a(vec_view<value_type>& a) { m_a = a; };
	/**
	 * @brief Set the middle vector
	 *
	 * @param b vector value
	 */
	void set_b(vec_view<value_type>& b) { m_b = b; };
	/**
	 * @brief Set upper vector
	 *
	 * @param c vector value
	 */
	void set_c(vec_view<value_type>& c) { m_c = c; };
	/**
	 * @brief Get constraints \f(d\f)
	 *
	 * @return constraints vector
	 */
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
