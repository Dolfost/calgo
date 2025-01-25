#ifndef _CALGO_INTERVAL_HPP_
#define _CALGO_INTERVAL_HPP_

#include <utility>
#include <cmath>
#include <ostream>
#include <type_traits>

namespace ca {

template<typename T>
struct Interval {
	static_assert(std::is_arithmetic<T>::value, "value_type should be arithmetic");
	using value_type = T;
	value_type a;
	value_type b;

	void swap() { std::swap(a, b); }
	value_type length() const { return std::abs(a-b); }
	value_type middle() const { return (a+b)/2; }
	bool inverted() const { return a > b; }

	template<typename O>
	friend std::ostream& operator<<(std::ostream& os, const Interval<O>& i) {
		return os << '[' << i.a << ", " << i.b << ']';
	}
};

}

#endif // !_CALGO_INTERVAL_HPP_
