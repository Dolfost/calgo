#ifndef _CALGO_VECTOR_HPP_
#define _CALGO_VECTOR_HPP_

#include <calgo/calgo.hpp>

#include <cstddef>
#include <vector>
#include <numeric>
#include <ostream>
#include <iomanip>

namespace ca {

template<typename T>

class Vector: public std::vector<T> {
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

	friend std::ostream& operator<<(std::ostream& os, const Vector<T> v) {
		for (int i = 0; i < v.size() - 1; i++)
			os << std::setw(9) << std::fixed << std::setprecision(4) << std::left << v[i];
		return os << v.back();
	}

	
};

}

#endif
