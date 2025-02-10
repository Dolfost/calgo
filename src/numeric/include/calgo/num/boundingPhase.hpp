#ifndef _CALGO_NUM_BOUNDING_PHASE_HPP_
#define _CALGO_NUM_BOUNDING_PHASE_HPP_

#include <calgo/calgo.hpp>
#include <calgo/interval.hpp>
#include <calgo/macros.hpp>

#include <functional>
#include <type_traits>
#include <cstddef>

namespace ca::num {

/**
 * @brief Bounding phase method
 *
 * This implementation takes functor \f(f(x)\f) and tries 
 * to isolate minima in some inerval \f([a,b]\f).
 * It is given the starting point and starting stem size,
 * then it determines in wich direction it should move, increasing the step 
 * by \f(2^i\f), where \f(i\f) is an iteration index.
 *
 * @tparam T `value_type`, should be signed arithmetic type
 */
template<typename T> 
class BoundingPhase {
	static_assert(std::is_signed<T>::value, "value_type should be signed");
public:
	using value_type = T;
	using interval_type = interval<T>;
	using function_type = std::function<value_type(value_type)>;
	using size_type = std::size_t;

	/**
	 * @brief Checks for invalid input
	 *
	 * @throws std::runtime_error
	 */
	void check();
	bool localize();
	bool localize_safe() { check(); return localize(); }

	template<typename P> typename 
	std::enable_if<std::is_assignable<function_type, P>::value>::type setFunction(P && function) {
		m_func = std::forward<P>(function);
	}
	const function_type& function() const { return m_func; };

	const interval_type& interval() const { return m_int; }
	bool found() const { return m_found; }

	size_type iterationLimit() const { return m_iterationLimit; }
	void setIterationLimit(const size_type& il) { m_iterationLimit = il; }

	void setStartingPoint(const value_type& sp) { m_startingPoint = sp; }
	value_type startingPoint() const { return m_startingPoint; }

	void setStep(const value_type& s) { m_step = s; }
	value_type step() const { return m_step; }

	CA_CALLBACK(
		iteration,
		value_type x0,
		value_type x1,
		value_type x2,
		value_type f0,
		value_type f1, 
		value_type f2,
		size_type it
	);


protected:
	function_type m_func;
	interval_type m_int;
	bool m_found;

	value_type m_startingPoint = 0;
	value_type m_step = 0.1;


	size_type m_iterationLimit = 16;
};

}

#include <calgo/nomacros.hpp>

#include "../../../src/boundingPhase.inl"

#endif // !_CALGO_NUM_BOUNDING_PHASE_HPP_
