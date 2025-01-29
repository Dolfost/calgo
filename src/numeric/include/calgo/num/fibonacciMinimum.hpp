#ifndef _CALGO_NUM_FIBONACCI_MINIMUM_HPP_
#define _CALGO_NUM_FIBONACCI_MINIMUM_HPP_

#include <calgo/num/intervalMinimum.hpp>
#include <calgo/macros.hpp>

#include <cstddef>

namespace ca::num {

/**
 * @brief Find minimum of function on interval using Fibonacci sequence in fixed number of iterations
 *
 * @tparam `value_type`
 */
template<typename T>
class FibonacciMinimum: public IntervalMinimum<T> {
public:
	using typename IntervalMinimum<T>::value_type;
	using typename IntervalMinimum<T>::function_type;
	using typename IntervalMinimum<T>::interval_type;
	using size_type = std::size_t;

public:
	using IntervalMinimum<T>::IntervalMinimum;

	void find() override;

	const size_type& iterations() const { return m_it; }
	void setIterations(const size_type& i) { m_it = i; }

	CA_CALLBACK(
		iteration,
		const interval_type& i,
		size_type it
	);

	static value_type fib(value_type n);

protected:
	size_type m_it = 10;
};

}

#include "../../../src/fibonacciMinimum.inl"

#endif // !_CALGO_NUM_FIBONACCI_MINIMUM_HPP_
