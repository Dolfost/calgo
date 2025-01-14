#ifndef _CALGO_VECTOR_HPP_
#define _CALGO_VECTOR_HPP_

#include <calgo/calgo.hpp>

#include <cstddef>
#include <ostream>
#include <type_traits>
#include <initializer_list>
#include <cstddef>

namespace ca {

template<typename T>
class Mat;

/**
 * @brief Array viev
 *
 * This class kepps view on the array that can be represented as pointer to
 * `VecView::value_type` array of size VecView::n(). Data cannot be freed, but
 * can be changed, though.
 *
 * @tparam T `value_type`
 * @sa Vec
 */
template<typename T>
class VecView {
	static_assert(std::is_arithmetic<T>(), "Not an arithmetic type");
public:
	using value_type = T;
	using size_type = std::size_t;

	/**
	 * @brief Constructor
	 *
	 * @param data pointer to `len` `value_type` entries 
	 * @param len data length
	 */
	VecView(value_type* data, size_type len);

	/**
	 * @brief Array length
	 *
	 * @return array length
	 */
	size_type n() const;

	/**
	 * @brief Dot product
	 *
	 * If `other` is not the same size, then
	 * behaivour is undefined.
	 *
	 * @param array of same size
	 * @return dot product value
	 */
	T operator*(const VecView<T>& other) noexcept;

	/**
	 * @brief Subscript operator
	 *
	 * If index is out of range, then behaivour is 
	 * undefined.
	 *
	 * @param idx index
	 * @return value reference
	 */
	value_type& operator[](const size_type& idx) noexcept;
	const value_type& operator[](const size_type& idx) const noexcept;

	/**
	 * @brief Smart subscript operator
	 *
	 * @param idx index
	 * @return value reference
	 * @throws std::out_of_range
	 */
	value_type& at(const size_type& idx);
	const value_type& at(const size_type& idx) const;

	/**
	 * @brief Scalar multiplication
	 *
	 * Multiplies each element of array by `value`
	 *
	 * @tparam M scalar type
	 * @param d scalar value
	 */
	template<typename M>
	void operator*=(const M& d) noexcept;
	/**
	 * @brief Smart dot product
	 *
	 * @copydetails operator*(const VecView<T>&)
	 * @throws std::runtime_error
	 */
	T dot(const VecView<T>& other);

	friend ca::Mat<T>;

	/**
	 * @brief STL output operator
	 *
	 * @tparam D `value_type`
	 * @param os output stream
	 * @param v array object
	 * @return passes `os` object
	 */
	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const VecView<D>& v);

protected:
	value_type* m_vec = nullptr; ///< data pointer
	size_type m_len = 0; ///< data length
};

/**
 * @brief Array class
 *
 * This class wraps VecView and defines memory management routines. It owns
 * it's data `Vec::m_vec`, so it will be freed when object is destroyed.
 *
 * @tparam T `value_type`
 * @sa VecView
 */
template<typename T>
class Vec: public VecView<T> {
public:
	using value_type = typename VecView<T>::value_type;
	using size_type = typename VecView<T>::size_type;

	using VecView<T>::VecView;

	/**
	 * @brief Make array object out of std::initializer_list
	 *
	 * @param data input list
	 */
	Vec(std::initializer_list<value_type> data = {});
	/**
	 * @brief Construct array of fixed size
	 *
	 * @param len array size
	 */
	Vec(const size_type& len);
	/**
	 * @brief Copy constructor
	 *
	 * @param other other object
	 */
	Vec(const Vec<value_type>& other);
	/**
	 * @brief Copy assignment operator
	 *
	 * @param other other object
	 */
	Vec& operator=(const Vec& other);
	/**
	 * @brief Resize array
	 *
	 * Elements of old array that fit into new one will be copied to it if `copy=true`. If
	 * old array if smaller than new, rest of the new elements will be
	 * initialized to 0.
	 * If copy if `false`, then value of each element is undefined. 
	 *
	 * @param rows new length
	 * @param copy will copy old matrix to new if `true`
	 */
	void resize(const size_type& len, bool copy = false);
	/**
	 * @brief Set each element to some value
	 *
	 * @param val value of each element
	 */
	void set(const value_type& val);

	friend ca::Mat<T>;

	/**
	 * @brief Destructor
	 *
	 * Deletes the data under Vec::m_vec pointer
	 */
	~Vec();
};

}

#include "../../src/vec.inl"

#endif
