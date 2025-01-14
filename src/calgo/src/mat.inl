#pragma once
#include <calgo/mat.hpp>

#include <stdexcept>
#include <algorithm>

#define CALGO_MATRIX_INDEX(mat, cols, row, col) ((mat) + (row)*cols + (col))

namespace ca {

template<typename T>
Mat<T>::Mat(std::initializer_list<std::initializer_list<value_type>> data) {
	m_cols = std::max_element(
		data.begin(), data.end(), [](auto a, auto b) {
			return a.size() > b.size();
		}
	)->size();

	m_rows = data.size();
	m_mat = new value_type[m_rows*m_cols];
	std::size_t col, row = 0;
	for (const auto& drow: data) {
		col = 0;
		for (const auto& elem: drow) {
			*CALGO_MATRIX_INDEX(m_mat, m_cols, row, col) = elem;
			col++;
		}
		row++;
	}
}

template<typename T>
Mat<T>::Mat(const std::size_t& rows, const std::size_t& cols) {
	m_rows = rows;
	m_cols = cols;
	m_mat = new value_type[m_rows*m_cols];
}

template<typename T>
Mat<T>::Mat(
	const std::size_t& rows, 
	const std::size_t& cols, 
	const value_type& init
): Mat(rows, cols) {
	for (std::size_t i = 0; i < m_rows; i++)
		for (std::size_t j = 0; j < m_cols; j++)
			*CALGO_MATRIX_INDEX(m_mat, m_cols, i, j) = init;
}

template<typename T>
Mat<T>::Mat(const Mat<T>& other): Mat(other.m_rows, other.m_cols) {
	std::size_t size = m_cols*m_rows;
	for (std::size_t i = 0; i < size; i++)
		m_mat[i] = other.m_mat[i];
}

template<typename T>
Mat<T>& Mat<T>::operator=(const Mat<T>& other) {
	delete[] m_mat;
	m_cols = other.m_cols;
	m_rows = other.m_rows;
	m_mat = new value_type[m_cols*m_rows];
	std::size_t size = m_cols*m_rows;
	for (std::size_t i = 0; i < size; i++)
		m_mat[i] = other.m_mat[i];
	return *this;
}

template<typename T>
void Mat<T>::set(const value_type& val) {
	value_type size = m_rows*m_cols;
	for (value_type i = 0; i < size; i++)
		m_mat[i] = val;
}

template<typename T>
void Mat<T>::resize(const size_type& rows, const size_type& cols, bool copy) {
	if (rows == m_rows and cols == m_cols)
		return;

	size_type size = rows*cols;
	if (copy) {
		value_type* newMat = new value_type[size];
		size_type stopRow = std::min(rows, m_rows);
		size_type stopCol = std::min(cols, m_cols);
		for (size_type i = 0; i < stopRow; i++)
			for (size_type j = 0; j < stopCol; j++)
				*CALGO_MATRIX_INDEX(newMat, cols, i, j) = 
					*CALGO_MATRIX_INDEX(m_mat, m_cols, i, j);
		for (size_type i = 0; i < rows - stopRow; i++)
			for (size_type j = 0; j < cols - stopCol; j++)
				*CALGO_MATRIX_INDEX(newMat, cols, i, j) = 0;
		delete[] m_mat;
		m_mat = newMat;
	} else {
		delete[] m_mat;
		m_mat = new value_type[size];
	}

	m_rows = rows;
	m_cols = cols;
}

template<typename T>
VecView<typename Mat<T>::value_type> Mat<T>::operator[](const std::size_t& row) noexcept {
	return VecView(CALGO_MATRIX_INDEX(m_mat, m_cols, row, 0), m_cols);
}

template<typename T>
const VecView<typename Mat<T>::value_type> Mat<T>::operator[](const std::size_t& row) const noexcept {
	return VecView(CALGO_MATRIX_INDEX(m_mat, m_cols, row, 0), m_cols);
}

template<typename T>
typename Mat<T>::value_type& Mat<T>::operator()(const std::size_t& row, const std::size_t& col) noexcept {
	return *CALGO_MATRIX_INDEX(m_mat, m_cols, row, col);
}

template<typename T>
const typename Mat<T>::value_type& Mat<T>::operator()(const std::size_t& row, const std::size_t& col) const noexcept {
	return *CALGO_MATRIX_INDEX(m_mat, m_cols, row, col);
}

template<typename T>
VecView<typename Mat<T>::value_type> Mat<T>::at(const std::size_t& row) noexcept {
	return operator[](row);
}

template<typename T>
const VecView<typename Mat<T>::value_type> Mat<T>::at(const std::size_t& row) const noexcept {
	return operator[](row);
}

template<typename T>
VecView<typename Mat<T>::value_type> Mat<T>::operator()(const std::size_t& row) {
	if (row >= m_rows)
		throw std::out_of_range("ca::Matrix row out of range");
	return operator[](row);
}

template<typename T>
const VecView<typename Mat<T>::value_type> Mat<T>::operator()(const std::size_t& row) const {
	if (row >= m_rows)
		throw std::out_of_range("ca::Matrix row out of range");
	return operator[](row);
}

template<typename T>
const typename Mat<T>::value_type& Mat<T>::at(const std::size_t& row, const std::size_t& col) const {
	if (row >= m_rows)
		throw std::out_of_range("ca::Matrix row out of range");
	if (col >= m_cols)
		throw std::out_of_range("ca::Matrix column out of range");
	return operator()(row, col);
}

template<typename T>
typename Mat<T>::value_type& Mat<T>::at(const std::size_t& row, const std::size_t& col) {
	if (row >= m_rows)
		throw std::out_of_range("ca::Matrix row out of range");
	if (col >= m_cols)
		throw std::out_of_range("ca::Matrix column out of range");
	return operator()(row, col);
}

template<typename T>
template<typename D>
std::ostream& Mat<T>::showSystem(const VecView<D>& v, std::ostream& os) const {
	if (m_rows != v.m_len)
		throw std::runtime_error("ca::Matrix: can not print system");

	for (std::size_t i = 0; i < v.m_len; i++)
		os << operator[](i) << "\t | " << v[i] << '\n';
	return os;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const Mat<D>& m) {
	for (typename Mat<D>::size_type i = 0; i < m.m_rows; i++) {
		os << m[i] << '\n';
	}
	return os;
}

template<typename T>
void ca::Mat<T>::insertRows(const size_type& row, const size_type& count, const value_type& init) {
	if (row > m_rows)
		throw std::out_of_range("ca::Matrix: inserted row is out of bounds");

	size_type newRows = m_rows + count;
	value_type* newMat = new value_type[newRows*m_cols];

	for (size_type i = 0; i < row; ++i)
		for (size_type j = 0; j < m_cols; ++j)
			*CALGO_MATRIX_INDEX(newMat, m_cols, i, j) = 
				*CALGO_MATRIX_INDEX(m_mat, m_cols, i, j);

	for (size_type i = row; i < row + count; ++i)
		for (size_type j = 0; j < m_cols; ++j)
			*CALGO_MATRIX_INDEX(newMat, m_cols, i, j) = init;

	for (size_type i = row + count; i < newRows; ++i)
		for (size_type j = 0; j < m_cols; ++j)
			*CALGO_MATRIX_INDEX(newMat, m_cols, i, j) = 
				*CALGO_MATRIX_INDEX(m_mat, m_cols, i - count, j);

	delete[] m_mat;
	m_mat = newMat;
	m_rows = newRows;
}

template<typename T>
void ca::Mat<T>::removeRows(const size_type& row, const size_type& count) {
	if (row + count > m_rows)
		throw std::out_of_range("ca::Matrix: removed row is out of bounds");

	size_type newRows = m_rows - count;
	value_type* newMat = new value_type[newRows*m_cols];

	for (size_type i = 0; i < row; ++i)
		for (size_type j = 0; j < m_cols; ++j)
			*CALGO_MATRIX_INDEX(newMat, m_cols, i, j) = 
				*CALGO_MATRIX_INDEX(m_mat, m_cols, i, j);

	for (size_type i = row; i < newRows; ++i)
		for (size_type j = 0; j < m_cols; ++j)
			*CALGO_MATRIX_INDEX(newMat, m_cols, i, j) = 
				*CALGO_MATRIX_INDEX(m_mat, m_cols, i + count, j);

	delete[] m_mat;
	m_mat = newMat;
	m_rows = newRows;
}

template<typename T>
void ca::Mat<T>::insertCols(const size_type& col, const size_type& count, const value_type& init) {
	if (col > m_cols)
		throw std::out_of_range("ca::Matrix: inserted columnt is out of bounds");

	size_type newCols = m_cols + count;
	value_type* newMat = new value_type[m_rows*newCols];

	for (size_type i = 0; i < m_rows; ++i) {
		for (size_type j = 0; j < col; ++j)
			*CALGO_MATRIX_INDEX(newMat, newCols, i, j) =
				*CALGO_MATRIX_INDEX(m_mat, m_cols, i, j);

		for (size_type j = col; j < col + count; ++j)
			*CALGO_MATRIX_INDEX(newMat, newCols, i, j) = init;

		for (size_type j = col + count; j < newCols; ++j)
			*CALGO_MATRIX_INDEX(newMat, newCols, i, j) = 
				*CALGO_MATRIX_INDEX(m_mat, m_cols, i, j - count);
	}

	delete[] m_mat;
	m_mat = newMat;
	m_cols = newCols;
}

template<typename T>
void ca::Mat<T>::removeCols(const size_type& col, const size_type& count) {
	if (col + count > m_cols)
		throw std::out_of_range("ca::Matrix: removed columnt is out of bounds");

	size_type newCols = m_cols - count;
	value_type* newMat = new value_type[m_rows*newCols];

	for (size_type r = 0; r < m_rows; ++r) {
		for (size_type c = 0; c < col; ++c)
			newMat[r * newCols + c] = m_mat[r * m_cols + c];

		for (size_type c = col; c < newCols; ++c)
			newMat[r * newCols + c] = m_mat[r * m_cols + (c + count)];
	}

	delete[] m_mat;
	m_mat = newMat;
	m_cols = newCols;
}

template<typename T>
Mat<T>::~Mat() {
	delete[] m_mat;
}

}

#undef CALGO_MATRIX_INDEX
