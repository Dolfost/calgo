#ifndef _CALGO_VECTOR_HPP_
#define _CALGO_VECTOR_HPP_

#include <calgo/calgo.hpp>

#include <cstddef>
#include <ostream>
#include <iostream>
#include <type_traits>
#include <initializer_list>
#include <cstddef>

namespace ca {

template<typename T>
class mat_view;
template<typename T>
class mat;
template<typename T>
class vec;

/**
 * @brief Array view
 *
 * This class kepps view on the array that can be represented as pointer to
 * `vec_view::value_type` array of size vec_view::n(). Data cannot be freed, but
 * can be changed, though.
 *
 * @tparam T `value_type`
 * @sa vec
 */
template<typename T>
class vec_view {
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
	vec_view(value_type* data = nullptr, const size_type& len = 0, const size_type& dist = 1);

	/**
	 * @brief Array length
	 *
	 * @return array length
	 */
	size_type n() const;

	/**
	 * @brief Sum of all elements
	 *
	 * @return sum of all elements
	 */
	virtual value_type sum() const;

	/**
	 * @brief Elementvise substraction
	 * If rhs is longer than lhs, behaivour is undefined.
	 * @tparam V `value_type` of other vector
	 * @param rhs right hand side 
	 * @return ca::vec<V> with size of the lhs vector
	 */
	template<typename V>
	auto operator-(const vec_view<V>& rhs) noexcept 
	-> ca::vec<decltype(std::declval<value_type>() - std::declval<typename vec_view<V>::value_type>())>;
	/** @brief Elementvise addition
	 * If rhs is longer than lhs, behaivour is undefined.
	 * @tparam V `value_type` of other vector
	 * @param rhs right hand side 
	 * @return ca::vec<V> with size of the lhs vector
	 */
	template<typename V>
	auto operator+(const vec_view<V>& rhs) noexcept 
	-> ca::vec<decltype(std::declval<value_type>() + std::declval<typename vec_view<V>::value_type>())>;
	/**
	 * @brief Dot product
	 *
	 * If `other` is not the same size, then
	 * behaivour is undefined.
	 *
	 * @param other vector of same size
	 * @return dot product value
	 */
	T operator*(const vec_view<T>& other) noexcept;
	/**
	 * @brief Cross product in 3D space
	 * If lhs size is not equal rhs and not equal to 3 - behaivour is undefined.
	 * @param rhs right hand side vector
	 * @tparam rhs_value_type rhs value_type
	 * @return cross product vector that is perpendicular to lhs and rhs vectors
	 */
	template<typename rhs_value_type>
	vec<decltype(std::declval<rhs_value_type>()*std::declval<typename ca::vec_view<T>::value_type>())> cross_3d(const vec_view<rhs_value_type>& rhs) noexcept;

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
	virtual void set(const value_type& val);

	/**
	 * @brief Product of all elements
	 *
	 * Product of an empty vector equals 1
	 * @return product of all elements
	 */
	value_type prod() const noexcept;

	/**
	 * @brief Get vector length
	 * Length \f(l\f) is calculated accordint to formula 
	 * \f{ l = \sqrt(e_1^2 + e_2^2 + \dots + \e_{n-1}^2 + e_n^2) \f}
	 * @tparam V return type
	 * @return vector length
	 */
	template<typename V = value_type>
	V length() const;
	/**
	 * @brief Normalize vector
	 * Makes vector length equal to 1
	 * @tparam V type used during calculations
	 */
	template<typename V = value_type>
	void normalize();
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
	 * @copydetails operator*(const vec_view<T>&)
	 * @throws std::runtime_error
	 */
	T dot(const vec_view<T>& other);
	/**
	 * @brief Comparison operator
	 *
	 * @param other other object to compare to 
	 * @return `true` if lengths and elements are same, `false` otherwise
	 */
	template<typename rhs_value_type>
	bool operator==(const vec_view<rhs_value_type>& other);
	template<typename rhs_value_type>
	inline bool operator!=(const vec_view<rhs_value_type>& other) {
		return not operator==<rhs_value_type>(other);
	}

	friend ca::mat<T>;
	friend ca::vec<T>;
	friend ca::mat_view<T>;

	/**
	 * @brief STL output operator
	 *
	 * @tparam D `value_type`
	 * @param os output stream
	 * @param v array object
	 * @return passes `os` object
	 */
	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const vec_view<D>& v);
	/**
	 * @brief Print vector as array
	 *
	 * @param os output stream
	 * @return passes `os` object
	 */
	std::ostream& as_array(std::ostream& os = std::cout) const;

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
 * This class wraps vec_view and defines memory management routines. It owns
 * it's data `vec::m_vec`, so it will be freed when object is destroyed.
 *
 * @tparam T `value_type`
 * @sa vec_view
 */
template<typename T>
class vec: public virtual vec_view<T> {
public:
	using typename vec_view<T>::value_type;
	using typename vec_view<T>::size_type;

	using vec_view<T>::vec_view;

	/**
	 * @brief Make array object out of std::initializer_list
	 *
	 * @param data input list
	 */
	vec(std::initializer_list<value_type> data = {});
	/**
	 * @brief Construct array of fixed size
	 *
	 * @param len array size
	 */
	vec(const size_type& len);
	/**
	 * @brief Construct array of fixed size and set it's elemets to some value
	 *
	 * @param len array size
	 * @param init default cell value
	 */
	vec(const size_type& len, const value_type& init);
	/**
	 * @brief Copy constructor
	 *
	 * @param other other object
	 */
	vec(const vec_view<value_type>& other);
	vec(const vec<value_type>& other): vec(static_cast<const vec_view<value_type>&>(other)) {};
	/**
	 * @brief Templated copy constructor
	 * Performs `static_cast<value_type>` on each element of `other`
	 * @tparam V `value_type` of `other` 
	 * @param other other vector
	 */
	template<typename V>
	vec(const vec_view<V>& other);
	template<typename V>
	vec(const vec<V>& other): vec(static_cast<const vec_view<V>&>(other)) {};

	/**
	 * @brief Move constructor
	 *
	 * @warning Do not move-costruct vec from vec_view if You don't know what You
	 * are doing. Data pointer that were stolen from vec_view may be owned by
	 * onther vec, so on destruction pointer will be freed two times.
	 *
	 * @param other other object rvalue reference
	 */
	vec(vec_view<value_type>&& other);
	vec(vec<value_type>&& other): vec(static_cast<vec_view<value_type>&&>(other)) {};
	/**
	 * @brief Copy assignment operator
	 *
	 * @param other other object
	 */
	vec& operator=(const vec_view<value_type>& other);
	vec& operator=(const vec<value_type>& other) { 
		return operator=(static_cast<const vec_view<value_type>&>(other)); 
	}
	/**
	 * @brief Move assignment operator
	 * @copydetails vec(const vec_view<value_type>&& other)
	 */
	vec& operator=(vec_view<value_type>&& other);
	vec& operator=(vec<value_type>&& other) { 
		return operator=(static_cast<vec_view<value_type>&&>(other)); 
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
	virtual void resize(const size_type& len, bool copy = false);

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
	 * `where` is vec::n(), the elements are appended to any existing in the
	 * array.
	 *
	 * @param where element to insert before
	 * @param count new element count
	 * @param init default element value
	 */
	virtual void insert(const size_type& where, const size_type& count, const value_type& init = 0);
	/**
	 * @brief Remove elements
	 * Removes `count` rows starting with the given `row`.
	 *
	 * @param from first element
	 * @param count element count
	 */
	virtual void remove(const size_type& from, const size_type& count);
	/// @}

	friend ca::mat<T>;
	friend ca::mat_view<T>;
	friend ca::vec_view<T>;

	/**
	 * @brief Destructor
	 *
	 * Deletes the data under vec::m_vec pointer
	 */
	~vec();
};

}

#include "../../src/vec.inl"

#endif
