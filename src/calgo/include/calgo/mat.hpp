#ifndef _CALGO_MAT_HPP_
#define _CALGO_MAT_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>

#include <initializer_list>
#include <iostream>
#include <ostream>
#include <type_traits>
#include <functional>

namespace ca {

/**
 * @brief Matrix view
 *
 * Class holds pointer to some data, can access and modify it, but not
 * realocate or free it. It is intended that underlyng data will be continious,
 * unlike using `std::vector<std::vector<value_type>>`. This means that already
 * implemented funcitons for vector dot product can be used to multiply row and
 * column (or two columns (see row() and col())) of same matrix without
 * allocating new memory by using `vec_view` that points to same data with
 * offset. For exammple to get first row from matrix \f(n\f) by \f(m\f) you
 * just make vector view that points to first element and allows access up to
 * \f(m\f)-th element. This would even work with `std::vector<std::vector<>>`.
 * But if You wan to do the same with colums of an matrix and work with matrix
 * columns as with plain arrays - you can't do this without saving adress of
 * every `std::vector` for each element. But `ca::vec_view` can do it. It just
 * saves the adress of first element in targeted column, number of elements to
 * skip to get to next element in the column and the total size of column
 * (allowed number of skips). Heck, You can do this with the matrices too! You
 * can have big matrix and get its submatrix with ca::mat_view::submat() and
 * work with it as with regular matrix without pain of counting offsets in the
 * big matrix!
 *
 * @tparam T `value_type`
 * @sa mat
 */
template<typename T>
class mat_view {
	static_assert(std::is_arithmetic<T>(), "Not an arithmetic type");
public:
	using value_type = T;
	using size_type = std::size_t;

	/**
	 * @brief Construct new object
	 * @param data `value_type` pointer
	 * @param rows number of rows in matrix
	 * @param cols number of columns in matrix
	 * @param dist distance between last element of one row and first element of
	 * next one
	 */
	mat_view(
		value_type* data = nullptr, 
		const size_type& rows = 0, 
		const size_type& cols = 0, 
		const size_type& dist = 1
	);

	/** @name Utility functions
	 * @{
	 * @brief Set each element to some value
	 * @param val value of each element
	 */
	virtual void set(const value_type& val);
	/**
	 * @brief Row count
	 * @return row count
	 */
	size_type rows() const { return this->m_rows; }
	/**
	 * @brief Column count
	 * @return column count
	 */
	size_type cols() const { return this->m_cols; }

	/**
	 * @brief Equality operator
	 * @param other other mat_view
	 * @return `true` if size of matrices and corresponding elements are equal
	 */
	template<typename rhs_value_type>
	bool operator==(const mat_view<rhs_value_type>& other) const noexcept;
	template<typename rhs_value_type>
	bool operator!=(const mat_view<rhs_value_type>& other) const noexcept {
		return not (*this == other);
	};
	/**
	 * @brief Compare matrices with custom comparator
	 * @tparam rhs_value_type `value_type` of argument matrix
	 * @param other argument matrix
	 * @param comp comparisment functor
	 * @return `true` if `comp` returnss `true` for all pairs of corresponding elements, `false` otherwise
	 */
	template<typename rhs_value_type, class comparator>
	typename std::enable_if<std::is_invocable_r<bool, comparator, value_type, rhs_value_type>::value, bool>::type 
	compare(const mat_view<rhs_value_type>& other, const comparator& comp) const noexcept;

	/**
	 * @brief Get data pointer
	 * @return internal data pointer
	 */
	value_type* data() { return this->m_mat; }
	const value_type* data() const { return this->m_mat; }
	/// @}

	/**
	* @name Printing functions 
	* @{
	* @brief STL stream output operator
	* @param os stream object
	* @param m matrix object
	* @return passes `os` object reference
	*/
	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const mat_view<D>& m);
	/**
	 * @brief Print matrix as array of initializer lists
	 * @tparam D matrix `value_type`
	 * @param os stream for output
	 * @return passed `os` stream reference
	 */
	std::ostream& as_array(std::ostream& os = std::cout) const;
	/**
	 * @brief Print matrix as system of equations
	 * @tparam D vector `value_type`
	 * @param v system constraints
	 * @param os stream for output
	 * @return passed `os` stream reference
	 */
	template<typename D>
	std::ostream& showSystem(const vec_view<D>& v, std::ostream& os = std::cout) const;
	/// @}


	/** @name Direct acces methods and operators
	 * When accessing matrix elements with methods below, out of range
	 * indices will not be caught and no exceptions will be thrown. This may be
	 * useful if you know that your will not access any forbidden addresses in
	 * advance, so You could save a little time of conditionals.
	 *
	 * Out of range indices lead to undefined behaivour.
	 *
	 * @param row matrix row
	 * @param col matrix col
	*/
	/// @{
	inline vec_view<value_type> operator[](const size_type& row) noexcept;
	inline const vec_view<value_type> operator[](const size_type& row) const noexcept;
	inline value_type& operator()(const size_type& row, const size_type& col) noexcept;
	inline const value_type& operator()(const size_type& row, const size_type& col) const noexcept;
	inline value_type& el(const size_type& row, const size_type& col) noexcept;
	inline const value_type& el(const size_type& row, const size_type& col) const noexcept;

	/**
	 * When accessing matrix elements with methods below, out of range
	 * indices will be caught and std::out_of_range exception will be thrown.
	 *
	 * @param row matrix row
	 * @param col matrix col
	 * @throws std::out_of_range
	*/
	/// @{
	/// @throws std::out_of_range
	vec_view<value_type> at(const size_type& row);
	/// @throws std::out_of_range
	const vec_view<value_type> at(const size_type& row) const;
	/// @throws std::out_of_range
	value_type& at(const size_type& row, const size_type& col);
	/// @throws std::out_of_range
	const value_type& at(const size_type& row, const size_type& col) const;
	/// @}
	
	/** @name Row/column/diagonal based vector view access
	 *
	 * Get the vector view at `at` row(column) starting from element `from` and
	 * spanning `len` elements. If `len` = 0, it is converted to vec::n().
	 * Any access out of range will lead to undefined behaivour.
	*/
	/// @{
	/**
	 * @brief Get vec_view at matrix row
	 * @param idx row index
	 * @param from starting index
	 * @param len view length
	 * @return vector view on a selected row
	 * @sa vec_view<value_type> operator()(const size_type& idx)
	 * @sa vec_view<value_type> at(const size_type& idx)
	 */
	vec_view<value_type> row(const size_type& at, const size_type& from = 0, const size_type& len = 0) noexcept;
	/**
	 * @brief Get vec_view at matrix column
	 * @param idx column index
	 * @param from starting index
	 * @param len view length
	 * @return vector view on a selected column
	 */
	vec_view<value_type> col(const size_type& at, const size_type& from = 0, const size_type& len = 0) noexcept;
	const vec_view<value_type> row(const size_type& at, const size_type& from = 0, const size_type& len = 0) const noexcept;
	const vec_view<value_type> col(const size_type& at, const size_type& from = 0, const size_type& len = 0) const noexcept;
	/// @throws std::out_of_range
	vec_view<value_type> row_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0);
	/// @throws std::out_of_range
	vec_view<value_type> col_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0);
	/// @throws std::out_of_range
	const vec_view<value_type> row_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0) const;
	/// @throws std::out_of_range
	const vec_view<value_type> col_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0) const;
	/**
	 * @brief Diagonal type
	 */
	enum class diagonal_type {
		lower, upper
	};
	/**
	 * @brief Get `vec_view` at any matrix diagonal
	 *
	 * @param index diagonal index. 0 means main diagonal
	 * @param dt diagonal location
	 * @sa diagonal_type
	 */
	vec_view<value_type> diagonal(const size_type& index, diagonal_type dt) noexcept;
	const vec_view<value_type> diagonal(const size_type& index, diagonal_type dt) const noexcept;
	vec_view<value_type> diagonal_safe(const size_type& index, diagonal_type dt);
	const vec_view<value_type> diagonal_safe(const size_type& index, diagonal_type dt) const;
	vec_view<value_type> diagonal_upper(const size_type& index) noexcept;
	const vec_view<value_type> diagonal_lower(const size_type& index) const noexcept;
	/// @throws std::out_of_range
	const vec_view<value_type> diagonal_upper_safe(const size_type& index) const;
	/// @throws std::out_of_range
	vec_view<value_type> diagonal_upper_safe(const size_type& index);
	/// @throws std::out_of_range
	const vec_view<value_type> diagonal_lower_safe(const size_type& index) const;
	/// @throws std::out_of_range
	vec_view<value_type> diagonal_lower_safe(const size_type& index);
	/// @brief Returns vec_view at the matrix main diagonal
	vec_view<value_type> diagonal() noexcept;
	const vec_view<value_type> diagonal() const noexcept;
	/// @}

	/** @name Matrix based view access
	 *
	 * `row`/`col` beiing out of range or `rows`/`cols` being too big leads to
	 * unexpected behaivour when calling methods without `_safe` suffix
	 *
	 * @brief Get mat_view at matrix
	 * @param row starting row
	 * @param col starting column
	 * @param rows submatrix height
	 * @param cols submatrix witdht
	 * Get the matrix view from (`row`,`col`) to (`row+rows`,`col+cols`) inclusive.
	 * @note If underlying matrix gets resized, acessing data under the view
	 * leads to undefined behaivour.
	 * @{
	 */
	mat_view<value_type> submat(
		const size_type& row,
		const size_type& col,
		const size_type& rows,
		const size_type& cols
	) noexcept;
	const mat_view<value_type> submat(
		const size_type& row,
		const size_type& col,
		const size_type& rows,
		const size_type& cols
	) const noexcept;
	/// @throws std::out_of_range
	mat_view<value_type> submmat_safe(
		const size_type& row,
		const size_type& col,
		const size_type& rows,
		const size_type& cols
	);
	/// @throws std::out_of_range
	const mat_view<value_type> submat_safe(
		const size_type& row,
		const size_type& col,
		const size_type& rows,
		const size_type& cols
	) const;
	/// @}


	/** 
	 * @name Matrix math
	 * @{
	 * @brief Matrix product related properties
	 * @tparam rhs `value_type` of right hand side 
	 */
	template<typename rhs> struct product { 
		using type = decltype(std::declval<value_type>() * std::declval<rhs>() + std::declval<value_type>() * std::declval<rhs>()); 
	}; 
	/**
	 * @brief Check if matrix can be multiplied by `other` 
	 * @tparam V `other` `value_type`
	 * @param other other matrix
	 * @return `true` if it can be multiplied, `false` otherwise
	 */
	template<typename V>
	bool is_product_comformable(const mat_view<V>& other) const { return m_cols == other.rows(); }
	template<typename V>
	bool is_product_comformable(const vec_view<V>& other) const { return m_cols == other.n(); };
	/**
	 * @brief Check if matrix is square matrix
	 * @return `true` if matrix is square, `false` otherwise
	 */
	inline bool is_square() const noexcept {
		return m_rows == m_cols;
	}
	 /**
	 * @brief Multiply matrices
	 * @tparam rhs_value_type `value_type` of argument matrix
	 * @param rhs multyply by
	 * @return multiplication result
	 */
	template<typename rhs_value_type> 
	auto mul(const ca::mat_view<rhs_value_type>& rhs) const noexcept -> ca::mat<typename product<rhs_value_type>::type>;
	template<typename rhs_value_type> 
	auto mul(const ca::vec_view<rhs_value_type>& rhs) const  noexcept -> ca::vec<typename product<rhs_value_type>::type>;
	template<typename rhs_value_type> 
	auto mul_safe(const ca::mat_view<rhs_value_type>& rhs) const -> ca::mat<typename product<rhs_value_type>::type>;
	template<typename rhs_value_type> 
	auto mul_safe(const ca::vec_view<rhs_value_type>& rhs) const-> ca::vec<typename product<rhs_value_type>::type>;
	template<typename rhs_value_type> 
	inline auto operator*(const ca::mat_view<rhs_value_type>& rhs) const noexcept -> ca::mat<typename product<rhs_value_type>::type> {
		return mul(std::forward(rhs));
	};
	/**
	 * @brief Fill matrix with coefficients of rotation in 2D space
	 *
	 * Fills the view starting from [0][0] with \f[
	 * \begin{bmatrix} \cos(\theta) & -\sin(\theta) \\ \sin(\theta) & \cos(\theta) \end{bmatrix}
	 * \f]
	 * If matrix is smaller than 2 by 2 - behaivour is undefined.
	 * @tparam rad_type argument type 
	 * @param rad radians to rotate to
	 */
	template<typename rad_type>
	void rotation2d(const rad_type& rad) noexcept;
	/**
	 * @brief Determinant
	 *
	 * Calculate a determinant by making the matrix an upper-triangular matrix
	 * swapping rows where needed and multyplying it's diagonal elements
	 * Method will not check if matrix is square
	 * @tparam V type to use during calculations
	 * @return determinant value
	 */
	template<typename V>
	typename std::enable_if<std::is_arithmetic<V>::value, V>::type det() const noexcept;
	/**
	 * @brief Same as `det()` but with a check for square matrix
	 * @copydetails det()
	 * @throws std::logic_error
	 */
	template<typename V>
	typename std::enable_if<std::is_arithmetic<V>::value, V>::type det_safe() const;
	/**
	 * @brief Find inverse matrix
	 *
	 * Finds inverse matrix using Gaussian elimination with partial pivoting
	 *
	 * This method won't check if matrix is square. If you need exception to be thrown when 
	 * the matrix isn't square -- see `ca::mat<T>::inverse_safe()`. 
	 * @throws std::runtime_error when matrix is singular
	 * @tparam V `value_type` of inverse matrix 
	 * @return inverse matrix (`ca::mat<V>`)
	 */
	template<typename V>
	typename std::enable_if<std::is_arithmetic<V>::value, mat<V>>::type inverse() const;
	/// @throws std::logic_error when matrix is not square
	/// @throws std::runtime_error when matrix is singular
	template<typename V>
	typename std::enable_if<std::is_arithmetic<V>::value, mat<V>>::type inverse_safe() const;
	/**
	 * @brief Transponation
	 */
	mat<value_type> transpose() const noexcept;
	/// @}

	friend ::ca::mat<T>;

protected:
	/**
	 * @brief Get address to elemet
	 * @param row element row
	 * @param col element column
	 * @return adress of element at `[row][col]`
	 */
	inline value_type* addr(const size_type& row, const size_type& col) const noexcept;

protected:
	value_type* m_mat = nullptr; ///< data pointer
	size_type m_rows = 0, m_cols = 0, m_dist = 1;
};



/**
 * @brief Matrix template
 *
 * @tparam T `value_type`
 * @sa mat_view
 */
template<typename T>
class mat: public virtual mat_view<T> {
	static_assert(std::is_arithmetic<T>(), "Not an arithmetic type");
public:
	using value_type = typename mat_view<T>::value_type;
	using size_type = typename mat_view<T>::size_type;

	using mat_view<T>::mat_view;

	/**
	 * @brief Construct matrix from std::initializer_list
	 */
	mat(std::initializer_list<std::initializer_list<value_type>> = {});
	/**
	 * @brief Construct matrix of know size
	 * @param rows rows count
	 * @param cols cols count
	 */
	mat(const size_type& rows, const size_type& cols);
	/**
	 * @brief Construct matrix and initialize each element to value
	 * @param rows rows count
	 * @param cols cols count
	 * @param init initialization value
	 */
	mat(const size_type& rows, const size_type& cols, const value_type& init);
	/**
	 * @brief Copy constructor
	 * @param other other matrix
	 */
	mat(const mat_view<value_type>& other);
	mat(const mat<value_type>& other): mat(static_cast<const mat_view<value_type>&>(other)) {};
	/**
	 * @brief Templated copy constructor
	 * Performs `static_cast<value_type>` on each element of `other`
	 * @tparam V `value_type` of `other` 
	 * @param other other matrix
	 */
	template<typename V>
	mat(const mat_view<V>& other);
	template<typename V>
	mat(const mat<V>& other): mat(static_cast<const mat_view<V>&>(other)) {};
	/**
	 * @brief Move constructor
	 * @note If somebody is wondering why there is no `mat(mat_view<value_type>&&
	 * other)`, there is an reason for that. Suppose you want to make matrix
	 * object from the `ca::mat_view` that you have built with
	 * `ca::mat_view.submat()`. If you tried to copy-construct ca::mat from that
	 * view in the return statement of an fuction, then the return value
	 * optimization would move the view to the caller and deleted the original
	 * object that owned the view data. When caller scope comes to an end, this
	 * data would bee freed again, but in other point of an array (which is
	 * incorrect in it's nature). `ca::mat_view` move costructor existed prior to
	 * v4.1.5.4.
	 * @param other other matrix
	 */
	mat(mat<value_type>&& other);
	/**
	 * @brief Copy assignment operator
	 * @param other other matrix
	 */
	mat& operator=(const mat_view<value_type>& other);
	mat& operator=(const mat& other) {
		return operator=(static_cast<const mat_view<value_type>&>(other));
	};
	/**
	 * @brief Move assignment operator
	 * @param other other matrix
	 */
	mat& operator=(mat_view<value_type>&& other);
	mat& operator=(mat&& other) {
		return operator=(static_cast<mat_view<value_type>&&>(other));
	};

	/**
	 * @brief Resize matrix
	 *
	 * Elements of old matrix that fit into new matrix will be copied to it if `copy=true`. If
	 * old matrix if smaller than new one, rest of the new elements will be
	 * initialized to 0.
	 * If copy if `false`, then value of each element is undefined. 
	 *
	 * @param rows new row count
	 * @param cols new column count
	 * @param copy will copy old matrix to new if `true`
	 */
	void resize(const size_type& rows, const size_type& cols, bool copy = false);

	/** @name Qt-styled MVC-like column/row manipulator functions
	 *
	 * These functions are memory realocation hungry, so be careful with them.
	 * @{
	 * @brief Insert rows
	 *
	 * Inserts `count` rows into the matrix before the given `row`. If
	 * `row` is 0, the rows are prepended to any existing rows in matrix. If
	 * row is mat::rows(), the rows are appended to any existing rows in the
	 * matrix.
	 *
	 * @param row row to insert before
	 * @param count new row count
	 * @param init default element value
	 */
	void insert_rows(const size_type& row, const size_type& count, const value_type& init = 0);
	/**
	 * @brief Remove rows
	 *
	 * Removes `count` rows starting with the given `row`.
	 * @param row starting row
	 * @param count row count
	 */
	void remove_rows(const size_type& row, const size_type& count);
	/**
	 * @brief  Insert columns
	 *
	 * Inserts `count` new columns into matrix before the given `col`. If `col`
	 * is 0, the columns are prepended to any existing columns. If column is
	 * mat::cols(), the columns are appended to any existing columns.
	 *
	 * @param col last column (exclusive)
	 * @param count column count
	 * @param init default element value
	 */
	void insert_cols(const size_type& col, const size_type& count, const value_type& init = 0);
	/**
	 * @brief Remove columns
	 *
	 * Removes `count` columns starting with the given `col`.
	 * @param row starting column
	 * @param count column count
	 */
	void remove_cols(const size_type& row, const size_type& count);
	/// @}

	friend mat_view<T>;

	~mat();
};

}

#include "../../src/mat.inl"

#endif // !_CALGO_MAT_HPP_
