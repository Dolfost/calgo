#ifndef _CALGO_INTERVAL_HPP_
#define _CALGO_INTERVAL_HPP_

#include <utility>
#include <cmath>
#include <ostream>
#include <type_traits>

namespace ca {

template<typename T>
struct interval {
	static_assert(std::is_arithmetic<T>::value, "value_type should be arithmetic");
	using value_type = T;
	value_type a;
	value_type b;

	void swap() { std::swap(a, b); }
	value_type length() const { return std::abs(a-b); }
	value_type middle() const { return (a+b)/2; }
	bool is_inverted() const { return a > b; }
	inline bool contains(const value_type& c) const { return a <= c and c <= b; }
	inline bool strictly_contains(const value_type& c) const { return a < c and c < b; }

	friend std::ostream& operator<<(std::ostream& os, const interval<T>& i) {
		return os << '[' << i.a << ", " << i.b << ']';
	}
};

}

#endif // !_CALGO_INTERVAL_HPP_
