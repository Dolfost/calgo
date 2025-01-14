#ifndef _CALGO_MAT_HPP_
#define _CALGO_MAT_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>

#include <initializer_list>
#include <iostream>
#include <ostream>

namespace ca {

/**
 * @brief Matrix template
 *
 * @tparam T `value_type`
 */
template<typename T>
class Mat {
	static_assert(std::is_arithmetic<T>(), "Not an arithmetic type");
public:
	using value_type = T;
	using size_type = std::size_t;

	/**
	 * @brief Construct matrix from std::initializer_list
	 */
	Mat(std::initializer_list<std::initializer_list<value_type>> = {});
	/**
	 * @brief Construct matrix of know size
	 * @param rows rows count
	 * @param cols cols count
	 */
	Mat(const size_type& rows, const size_type& cols);
	/**
	 * @brief Construct matrix and initialize each element to value
	 * @param rows rows count
	 * @param cols cols count
	 * @param init initialization value
	 */
	Mat(const size_type& rows, const size_type& cols, const value_type& init);
	/**
	 * @brief Copy constructor
	 * @param other other matrix
	 */
	Mat(const Mat<value_type>& other);
	/**
	 * @brief Copy assignment operator
	 * @param other other matrix
	 * @return lvalue
	 */
	Mat& operator=(const Mat& other);

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
	/**
	 * @brief Set each element to some value
	 *
	 * @param val value of each element
	 */
	void set(const value_type& val);
	/**
	 * @brief Row count
	 * @return row count
	 */
	size_type rows() const { return m_rows; }
	/**
	 * @brief Column count
	 * @return column count
	 */
	size_type cols() const { return m_cols; }

	/**
	* @brief STL stream output operator
	* @param os stream object
	* @param m matrix object
	* @return passes `os` object reference
	*/
	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const Mat<D>& m);

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
	VecView<value_type> operator[](const size_type& row) noexcept;
	const VecView<value_type> operator[](const size_type& row) const noexcept;
	value_type& operator()(const size_type& row, const size_type& col) noexcept;
	const value_type& operator()(const size_type& row, const size_type& col) const noexcept;
	VecView<value_type> at(const size_type& row) noexcept;
	const VecView<value_type> at(const size_type& row) const noexcept;
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
	VecView<value_type> operator()(const size_type& row);
	const VecView<value_type> operator()(const size_type& row) const;
	value_type& at(const size_type& row, const size_type& col);
	const value_type& at(const size_type& row, const size_type& col) const;
	/// @}

	/** @name Qt-styled MVC-like column/row manipulator functions
	 *
	 * These functions are memory realocation hungry, so be careful with them.
	*/
	/// @{
	/**
	 * @brief Insert rows
	 *
	 * Inserts `count` rows into the model before the given `row`. If
	 * `row` is 0, the rows are prepended to any existing rows in matrix. If
	 * row is Mat::rows(), the rows are appended to any existing rows in the
	 * matrix.
	 *
	 * @param row row to insert before
	 * @param count new row count
	 * @param init default element value
	 */
	void insertRows(const size_type& row, const size_type& count, const value_type& init = 0);
	/**
	 * @brief Remove rows
	 * Removes `count` rows starting with the given `row`.
	 *
	 * @param row starting row
	 * @param count row count
	 */
	void removeRows(const size_type& row, const size_type& count);
	/**
	 * @brief  Insert columns
	 *
	 * Inserts `count` new columns into matrix before the given `col`. If `col`
	 * is 0, the columns are prepended to any existing columns. If column is
	 * Mat::cols(), the columns are appended to any existing columns.
	 *
	 * @param col last column (exclusive)
	 * @param count column count
	 * @param init default element value
	 */
	void insertCols(const size_type& col, const size_type& count, const value_type& init = 0);
	/**
	 * @brief Remove columns
	 * Removes `count` columns starting with the given `col`.
	 *
	 * @param row starting column
	 * @param count column count
	 */
	void removeCols(const size_type& row, const size_type& count);
	/// @}

	/**
	 * @brief Print matrix as system of equations
	 *
	 * @tparam D vector `value_type`
	 * @param v system constraints
	 * @param os stream for output
	 * @return passed `os` stream reference
	 */
	template<typename D>
	std::ostream& showSystem(const VecView<D>& v, std::ostream& os = std::cout) const;

	~Mat();

protected:
	value_type* m_mat = nullptr; ///< data pointer
	size_type m_rows = 0, m_cols = 0;
};

}

#include "../../src/mat.inl"

#endif // !_CALGO_MAT_HPP_
