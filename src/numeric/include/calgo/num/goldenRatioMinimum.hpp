#ifndef _CALGO_NUM_GOLDEN_RATIO_MINIMUM_HPP_
#define _CALGO_NUM_GOLDEN_RATIO_MINIMUM_HPP_

#include <calgo/num/intervalMinimum.hpp>
#include <calgo/macros.hpp>

#include <cstddef>

namespace ca::num {

/**
 * @brief Finds minimum on given interval using golden ratio
 *
 * @tparam T `value_type`
 */
template<typename T>
class GoldenRatioMinimum: public IntervalMinimum<T> {
public:
	using typename IntervalMinimum<T>::value_type;
	using typename IntervalMinimum<T>::function_type;
	using typename IntervalMinimum<T>::interval_type;
	using size_type = std::size_t;

public:
	using IntervalMinimum<T>::IntervalMinimum;

	void find() override;

	CA_CALLBACK(
		iteration,
		const interval_type& i,
		size_type it
	);
};

}

#include "../../../src/goldenRatioMinimum.inl"

#endif // !_CALGO_NUM_GOLDEN_RATIO_MINIMUM_HPP_
