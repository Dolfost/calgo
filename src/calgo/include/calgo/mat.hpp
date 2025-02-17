#ifndef _CALGO_MAT_HPP_
#define _CALGO_MAT_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>

#include <initializer_list>
#include <iostream>
#include <ostream>

namespace ca {

// template<typename T>
// class mat;

/**
 * @brief Matrix view
 *
 * Class holds pointer to some data, can access and modify it, but not
 * realocate or free it.
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
	 *
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

	/**
	 * @brief Set each element to some value
	 *
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
	 *
	 * @param other other mat_view
	 * @return `true` if size of matrices and corresponding elements are equal
	 */
	bool operator==(const mat_view<value_type>& other);
	bool operator!=(const mat_view<value_type>& other) {
		return not (*this == other);
	};

	/**
	 * @brief Get data pointer
	 * @return internal data pointer
	 */
	value_type* data() { return this->m_mat; }
	const value_type* data() const { return this->m_mat; }

	/**
	* @brief STL stream output operator
	* @param os stream object
	* @param m matrix object
	* @return passes `os` object reference
	*/
	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const mat_view<D>& m);
	/**
	 * @brief Print matrix as array of initializer lists
	 *
	 * @tparam D matrix `value_type`
	 * @param os stream for output
	 * @return passed `os` stream reference
	 */
	std::ostream& as_array(std::ostream& os = std::cout) const;
	/**
	 * @brief Print matrix as system of equations
	 *
	 * @tparam D vector `value_type`
	 * @param v system constraints
	 * @param os stream for output
	 * @return passed `os` stream reference
	 */
	template<typename D>
	std::ostream& showSystem(const vec_view<D>& v, std::ostream& os = std::cout) const;


	/** @name Direct acces methods and operators
	 * When accessing matrix elements with methods of this group, out of range
	 * indices will not be caught and no exceptions will be thrown. This may be
	 * useful if you know that your will not access any forbidden addresse in
	 * advance, so could save a little time of conditionals.
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
	/// @}

	/** @name Smart acces methods and operators
	 * When accessing matrix elements with methods of this group, out of range
	 * indices will be caught and std::out_of_range exception will be thrown.
	 *
	 * @param row matrix row
	 * @param col matrix col
	 * @throws std::out_of_range
	*/
	/// @{
	vec_view<value_type> at(const size_type& row);
	const vec_view<value_type> at(const size_type& row) const;
	value_type& at(const size_type& row, const size_type& col);
	const value_type& at(const size_type& row, const size_type& col) const;
	/// @}
	
	/** @name Row/column based vector view access
	 *
	 * Get the vector view at `at` row(column) starting from element `from` and
	 * spanning `len` elements. If `len` = 0, it is converted to vec::n().
	 * Any access out of range will lead to undefined behaivour.
	*/
	/// @{
	/**
	 * @brief Get vec_view at matrix row
	 *
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
	 *
	 * @param idx column index
	 * @param from starting index
	 * @param len view length
	 * @return vector view on a selected column
	 */
	vec_view<value_type> col(const size_type& at, const size_type& from = 0, const size_type& len = 0) noexcept;
	const vec_view<value_type> row(const size_type& at, const size_type& from = 0, const size_type& len = 0) const noexcept;
	const vec_view<value_type> col(const size_type& at, const size_type& from = 0, const size_type& len = 0) const noexcept;
	/// @}

	/** @name Smart row/column based vector view access
	 *
	 * Any access out of range will lead to std::out_of_range beeing thrown.
	*/
	/// @{
	/**
	 * @copydoc vec_view<value_type> row(const size_type& at, const size_type& from = 0, const size_type& len = 0) noexcept
	 * @throws std::out_of_range
	 */
	vec_view<value_type> row_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0);
	/**
	 * @copydoc vec_view<value_type> col(const size_type& at, const size_type& from = 0, const size_type& len = 0) noexcept
	 * @throws std::out_of_range
	 */
	vec_view<value_type> col_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0);
	const vec_view<value_type> row_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0) const;
	const vec_view<value_type> col_safe(const size_type& at, const size_type& from = 0, const size_type& len = 0) const;
	/// @}

	/**
	 * @brief Submatrix functions
	 *
	 * `row`/`col` beiing out of rance or `rows`/`cols` beiing too big leads to
	 * unexpected behaivour.
	 *
	 * @param row starting row
	 * @param col starting column
	 * @param rows submatrix height
	 * @param cols submatrix witdht
	 */

	/** @name Matrix based view access
	 *
	 * Get the matrix view from (`row`,`col`) to (`row+rows`,`col+cols`) inclusive.
	 * @note If underlying matrix gets resized, acessing data under the view
	 * leads to undefined behaivour.
	*/
	/// @{
	/**
	 * @brief Get mat_view at matrix
	 *
	 * @param row starting row
	 * @param col starting column
	 * @param rows row span size
	 * @param cols column span size
	 * @return matrix view on a selected area
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

	mat_view<value_type> submmat_safe(
		const size_type& row,
		const size_type& col,
		const size_type& rows,
		const size_type& cols
	) noexcept;
	const mat_view<value_type> submat_safe(
		const size_type& row,
		const size_type& col,
		const size_type& rows,
		const size_type& cols
	) const noexcept;
	/// @}

	/**
	 * @brief Check if matrix is square matrix
	 * @return `true` if matrix is square, `false` otherwise
	 */
	bool is_square() const {
		return m_rows == m_cols;
	}

	friend ::ca::mat<T>;

protected:
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
	 * @brief Move constructor
	 * @param other other matrix
	 */
	mat(mat_view<value_type>&& other);
	mat(mat<value_type>&& other): mat(static_cast<mat_view<value_type>&&>(other)) {};
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
	*/
	/// @{
	/**
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
	 * Removes `count` rows starting with the given `row`.
	 *
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
	 * Removes `count` columns starting with the given `col`.
	 *
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
