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
class MatView;
template<typename T>
class Mat;
template<typename T>
class Vec;

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
	 * @param dist distance between neighboring elements
	 */
	VecView(value_type* data = nullptr, const size_type& len = 0, const size_type& dist = 1);

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
	 * @param index element index
	 * @return value reference
	 */
	inline value_type& operator[](const size_type& index) noexcept;
	inline const value_type& operator[](const size_type& index) const noexcept;
	inline value_type& el(const size_type& index) noexcept;
	inline const value_type& el(const size_type& index) const noexcept;

	/**
	 * @brief Smart subscript operator
	 *
	 * @param index element index
	 * @return value reference
	 * @throws std::out_of_range
	 */
	value_type& at(const size_type& index);
	const value_type& at(const size_type& index) const;

	/**
	 * @brief Set each element to some value
	 *
	 * @param val value of each element
	 */
	void set(const value_type& val);

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
	friend ca::Vec<T>;
	friend ca::MatView<T>;

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
	inline value_type* addr(const size_type& idx) const noexcept;

protected:
	value_type* m_vec = nullptr; ///< data pointer
	size_type m_len = 0; ///< data length
	size_type m_dist = 1; ///< space between each value
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
	using typename VecView<T>::value_type;
	using typename VecView<T>::size_type;

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
	Vec(const Vec<value_type>& other): Vec(static_cast<const VecView<value_type>&>(other)) {};
	Vec(const VecView<value_type>& other);
	/**
	 * @brief Move constructor
	 *
	 * @warning Do not move-costruct Vec from VecView if You don't know what You
	 * are doing. Data pointer that were stolen from VecView may be owned by
	 * onther Vec, so on destruction pointer will be freed two times.
	 *
	 * @param other other object rvalue reference
	 */
	Vec(VecView<value_type>&& other);
	Vec(Vec<value_type>&& other): Vec(static_cast<VecView<value_type>&&>(other)) {};
	/**
	 * @brief Copy assignment operator
	 *
	 * @param other other object
	 */
	Vec& operator=(const VecView<value_type>& other);
	Vec& operator=(const Vec<value_type>& other) { 
		return operator=(static_cast<const VecView<value_type>&>(other)); 
	}
	/**
	 * @brief Move assignment operator
	 * @copydetails Vec(const VecView<value_type>&& other)
	 */
	Vec& operator=(VecView<value_type>&& other);
	Vec& operator=(Vec<value_type>&& other) { 
		return operator=(static_cast<VecView<value_type>&&>(other)); 
	}
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

	/** @name Qt-styled MVC-like element manipulator functions
	 *
	 * These functions are memory realocation hungry, so be careful with them.
	*/
	/// @{
	/**
	 * @brief Insert elements
	 *
	 * Inserts `count` elements into the array before the given `where`. If
	 * `where` is 0, elements are prepended to any existing elements in array. If
	 * `where` is Vec::n(), the elements are appended to any existing in the
	 * array.
	 *
	 * @param where element to insert before
	 * @param count new element count
	 * @param init default element value
	 */
	void insert(const size_type& where, const size_type& count, const value_type& init = 0);
	/**
	 * @brief Remove elements
	 * Removes `count` rows starting with the given `row`.
	 *
	 * @param from first element
	 * @param count element count
	 */
	void remove(const size_type& from, const size_type& count);
	/// @}

	friend ca::Mat<T>;
	friend ca::MatView<T>;
	friend ca::VecView<T>;

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
