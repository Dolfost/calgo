#ifndef _CALGO_VECTOR_HPP_
#define _CALGO_VECTOR_HPP_

#include <calgo/calgo.hpp>

#include <cstddef>
#include <vector>
#include <numeric>
#include <ostream>
#include <iomanip>
#include <type_traits>

namespace ca {

template<typename T>

class Vector: public std::vector<T> {
	static_assert(std::is_arithmetic<T>(), "Not an arithmetic type");
public:
	using std::vector<T>::vector;
	T sum() {
		return std::accumulate(
			std::vector<T>::cbegin(),
			std::vector<T>::cend(),
			0
		);
	};

	T operator*(const Vector<T>& other) {
		if (this->size() != other.size())
			throw std::runtime_error("dot product not possible");

		std::size_t s = this->size();
		T prod = 0;
		for (std::size_t i = 0; i < s; i++)
			prod += this->at(i)*other[i];
		
		return prod;
	}

	T dot(const Vector<T>& other) {
		return operator*(other);
	}

	template<typename M>
	auto operator*=(const M& d) {
		static_assert(std::is_arithmetic<M>(), "Not an arithmetic type");
		for (auto& x: *this) {
			x *= d;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector<T> v) {
		for (int i = 0; i < v.size() - 1; i++)
			os << std::setw(9) << std::fixed << std::setprecision(4) << std::left << v[i];
		return os << v.back();
	}

	
};

}

#endif
