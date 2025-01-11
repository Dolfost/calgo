#ifndef _CALGO_MATRIX_HPP_
#define _CALGO_MATRIX_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vector.hpp>

#include <ostream>

namespace ca {

template<typename T>
class Matrix: public std::vector<Vector<T>> {
public:
	using std::vector<Vector<T>>::vector;
	std::size_t rows() const {
		return std::vector<Vector<T>>::size();
	}
	std::size_t cols() const {
		return std::vector<Vector<T>>::size() ? this->at(0).size() : 0;
	}

	friend std::ostream& operator<<(std::ostream& os, const Matrix<T> m) {
		for (auto const& v: m) {
			os << v << '\n';
		}
		return os;
	}

	template<typename D>
	std::ostream& system(const Vector<D>& v, std::ostream& os) const {
		if (this->rows() != v.size())
			throw std::runtime_error("matrix: can not print system");

		for (std::size_t i = 0; i < v.size(); i++)
			os << this->at(i) << "\t | " << v[i] << '\n';
		return os;
	}
};

}

#endif // !_CALGO_MATRIX_HPP_
