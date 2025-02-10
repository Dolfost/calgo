#ifndef _CALGO_NUM_INTERVAL_MINIMUM_HPP_
#define _CALGO_NUM_INTERVAL_MINIMUM_HPP_

#include <calgo/interval.hpp>

#include <functional>

namespace ca::num {

/**
 * @brief Generic base class for  interval function minimization
 *
 * @tparam T `value_type`
 */
template<typename T>
class IntervalMinimum {
public:
	using value_type = T;
	using interval_type = interval<value_type>;
	using function_type = std::function<value_type(value_type)>;

public:
	IntervalMinimum(const interval_type& inter = {0, 1}): m_int(inter) {}

	const interval_type& interval() const { return m_int; };
	void setInterval(const interval_type& i) { m_int = i; };

	const value_type& epsilon() const { return m_eps; };
	void setEpsilon(const value_type& e) { m_eps = e; };

	template<typename P> typename 
	std::enable_if<std::is_assignable<function_type, P>::value>::type setFunction(P && function) {
		m_func = std::forward<P>(function);
	}
	const function_type& function() const { return m_func; };

	const value_type& x() const { return m_x; };

	virtual void find() = 0;
	virtual void check() const;
	void find_safe() { check(); find(); }

	virtual ~IntervalMinimum() {};
protected:
	interval_type m_int;
	function_type m_func;
	value_type m_x;
	value_type m_eps = 0.1;
};

}

#include "../../../src/intervalMinimum.inl"

#endif // !_CALGO_NUM_INTERVAL_MINIMUM_HPP_
