#pragma once
#include <calgo/mat.hpp>

#include <stdexcept>
#include <algorithm>

#define CALGO_MAT_INDEX(mat, cols, dist, row, col) ((mat) + (row)*((cols)+(dist)-1) + (col))

namespace ca {

template<typename T>
MatView<T>::MatView(
	value_type* data, 
	const size_type& rows, 
	const size_type& cols, 
	const size_type& dist
) {
	m_mat = data;
	m_rows = rows;
	m_cols = cols;
	m_dist = dist;
}

template<typename T>
Mat<T>::Mat(
	std::initializer_list<std::initializer_list<value_type>> data
): MatView<T>(nullptr, 0, 0) {
	if (not data.size())
		return;

	this->m_cols = std::max_element(
		data.begin(), data.end(), [](auto a, auto b) {
			return a.size() > b.size();
		}
	)->size();

	this->m_rows = data.size();
	this->m_mat = new value_type[this->m_rows*this->m_cols];
	size_type col, row = 0;
	for (const auto& drow: data) {
		col = 0;
		for (const auto& elem: drow) {
			this->el(row, col) = elem;
			col++;
		}
		row++;
	}
}

template<typename T>
Mat<T>::Mat(
	const size_type& rows, const size_type& cols
): MatView<T>(new value_type[rows*cols], rows, cols) {
}

template<typename T>
Mat<T>::Mat(
	const size_type& rows, 
	const size_type& cols, 
	const value_type& init
): Mat(rows, cols) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = init;
}

template<typename T>
Mat<T>::Mat(const MatView<value_type>& other): Mat(other.m_rows, other.m_cols) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = other.el(i, j);
}

template<typename T>
Mat<T>::Mat(MatView<value_type>&& other): Mat(other.m_mat, other.m_rows, other.m_cols, other.m_dist) {
	other.m_mat = nullptr;
	other.m_rows = 0;
	other.m_cols = 0;
	other.m_dist = 1;
}

template<typename T>
Mat<T>& Mat<T>::operator=(const MatView<value_type>& other) {
	delete[] this->m_mat;
	this->m_cols = other.m_cols;
	this->m_rows = other.m_rows;
	this->m_mat = new value_type[this->m_cols*this->m_rows];
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = other.el(i, j);
	return *this;

}

template<typename T>
Mat<T>& Mat<T>::operator=(MatView<value_type>&& other) {
	delete[] this->m_mat;
	this->m_cols = other.m_cols;
	this->m_rows = other.m_rows;
	this->m_mat = other.m_mat;
	this->m_dist = other.m_dist;
	other.m_mat = nullptr;
	other.m_rows = 0;
	other.m_cols = 0;
	other.m_dist = 1;
	return *this;
}

template<typename T>
void MatView<T>::set(const value_type& val) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = val;
}

template<typename T>
void Mat<T>::resize(const size_type& rows, const size_type& cols, bool copy) {
	if (rows == this->m_rows and cols == this->m_cols)
		return;

	size_type size = rows*cols;
	if (copy) {
		value_type* newMat = new value_type[size];
		size_type stopRow = std::min(rows, this->m_rows);
		size_type stopCol = std::min(cols, this->m_cols);
		for (size_type i = 0; i < stopRow; i++)
			for (size_type j = 0; j < stopCol; j++)
				*CALGO_MAT_INDEX(newMat, cols, 0, i, j) = 
					this->el(i, j);
		for (size_type i = 0; i < rows - stopRow; i++)
			for (size_type j = 0; j < cols - stopCol; j++)
				*CALGO_MAT_INDEX(newMat, cols, 0, i, j) = 0;
		delete[] this->m_mat;
		this->m_mat = newMat;
	} else {
		delete[] this->m_mat;
		this->m_mat = new value_type[size];
	}

	this->m_rows = rows;
	this->m_cols = cols;
	this->m_dist = 1;
}

template<typename T>
VecView<typename MatView<T>::value_type> MatView<T>::operator[](const size_type& row) noexcept {
	return VecView<value_type>(this->addr(row, 0), this->m_cols);
}

template<typename T>
const VecView<T> MatView<T>::operator[](const size_type& row) const noexcept {
	return VecView<value_type>(this->addr(row, 0), this->m_cols);
}

template<typename T>
typename MatView<T>::value_type* MatView<T>::addr(const size_type& row, const size_type& col) const noexcept {
	return CALGO_MAT_INDEX(m_mat, m_cols, m_dist, row, col);
}

template<typename T>
typename MatView<T>::value_type& MatView<T>::el(const size_type& row, const size_type& col) noexcept {
	return *addr(row, col);
}

template<typename T>
const typename MatView<T>::value_type& MatView<T>::el(const size_type& row, const size_type& col) const noexcept {
	return *addr(row, col);
}

template<typename T>
typename MatView<T>::value_type& MatView<T>::operator()(const size_type& row, const size_type& col) noexcept {
	return el(row, col);
}

template<typename T>
const typename MatView<T>::value_type& MatView<T>::operator()(const size_type& row, const size_type& col) const noexcept {
	return el(row, col);
}

template<typename T>
VecView<typename MatView<T>::value_type> MatView<T>::at(const size_type& row) {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	return operator[](row);
}

template<typename T>
const VecView<typename MatView<T>::value_type> MatView<T>::at(const size_type& row) const {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	return operator[](row);
}

template<typename T>
const typename MatView<T>::value_type& MatView<T>::at(const size_type& row, const size_type& col) const {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	if (col >= this->m_cols)
		throw std::out_of_range("ca::Mat column out of range");
	return el(row, col);
}

template<typename T>
typename MatView<T>::value_type& MatView<T>::at(const size_type& row, const size_type& col) {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	if (col >= this->m_cols)
		throw std::out_of_range("ca::Mat column out of range");
	return el(row, col);
}

template<typename T>
template<typename D>
std::ostream& MatView<T>::showSystem(const VecView<D>& v, std::ostream& os) const {
	if (this->m_rows != v.m_len)
		throw std::runtime_error("ca::Mat: can not print system");

	for (size_type i = 0; i < v.m_len; i++)
			os << row(i) << "\t | " << v[i] << '\n';
	return os;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const MatView<D>& m) {
	for (typename MatView<D>::size_type i = 0; i < m.m_rows; i++)
		os << m[i] << '\n';
	return os;
}

template<typename T>
void ca::Mat<T>::insertRows(const size_type& row, const size_type& count, const value_type& init) {
	if (row > this->m_rows)
		throw std::out_of_range("ca::Mat: inserted row is out of bounds");

	size_type newRows = this->m_rows + count;
	value_type* newMat = new value_type[newRows*this->m_cols];

	for (size_type i = 0; i < row; ++i)
		for (size_type j = 0; j < this->m_cols; ++j)
			*CALGO_MAT_INDEX(newMat, this->m_cols, 1, i, j) = this->el(i, j);

	for (size_type i = row; i < row + count; ++i)
		for (size_type j = 0; j < this->m_cols; ++j)
			*CALGO_MAT_INDEX(newMat, this->m_cols, 1, i, j) = init;

	for (size_type i = row + count; i < newRows; ++i)
		for (size_type j = 0; j < this->m_cols; ++j)
			*CALGO_MAT_INDEX(newMat, this->m_cols, 1, i, j) = this->el(i - count, j);

	delete[] this->m_mat;
	this->m_mat = newMat;
	this->m_rows = newRows;
	this->m_dist = 1;
}

template<typename T>
void ca::Mat<T>::removeRows(const size_type& row, const size_type& count) {
	if (row + count > this->m_rows)
		throw std::out_of_range("ca::Mat: removed row is out of bounds");

	size_type newRows = this->m_rows - count;
	value_type* newMat = new value_type[newRows*this->m_cols];

	for (size_type i = 0; i < row; ++i)
		for (size_type j = 0; j < this->m_cols; ++j)
			*CALGO_MAT_INDEX(newMat, this->m_cols, 1, i, j) = 
				this->el(i, j);

	for (size_type i = row; i < newRows; ++i)
		for (size_type j = 0; j < this->m_cols; ++j)
			*CALGO_MAT_INDEX(newMat, this->m_cols, 1, i, j) = 
				this->el(i + count, j);

	delete[] this->m_mat;
	this->m_mat = newMat;
	this->m_rows = newRows;
	this->m_dist = 1;
}

template<typename T>
void ca::Mat<T>::insertCols(const size_type& col, const size_type& count, const value_type& init) {
	if (col > this->m_cols)
		throw std::out_of_range("ca::Mat: inserted columnt is out of bounds");

	size_type newCols = this->m_cols + count;
	value_type* newMat = new value_type[this->m_rows*newCols];

	for (size_type i = 0; i < this->m_rows; ++i) {
		for (size_type j = 0; j < col; ++j)
			*CALGO_MAT_INDEX(newMat, newCols, 1, i, j) =
				this->el(i, j);

		for (size_type j = col; j < col + count; ++j)
			*CALGO_MAT_INDEX(newMat, newCols, 1, i, j) = init;

		for (size_type j = col + count; j < newCols; ++j)
			*CALGO_MAT_INDEX(newMat, newCols, 1, i, j) = 
				this->el(i, j - count);
	}

	delete[] this->m_mat;
	this->m_mat = newMat;
	this->m_cols = newCols;
	this->m_dist = 1;
}

template<typename T>
void ca::Mat<T>::removeCols(const size_type& col, const size_type& count) {
	if (col + count > this->m_cols)
		throw std::out_of_range("ca::Mat: removed columnt is out of bounds");

	size_type newCols = this->m_cols - count;
	value_type* newMat = new value_type[this->m_rows*newCols];

	for (size_type i = 0; i < this->m_rows; ++i) {
		for (size_type j = 0; j < col; ++j)
			*CALGO_MAT_INDEX(newMat, newCols, 1, i, j) = 
				this->el(i, j);

		for (size_type j = col; j < newCols; ++j)
			*CALGO_MAT_INDEX(newMat, newCols, 1, i, j) = 
				this->el(i, j + count);
	}

	delete[] this->m_mat;
	this->m_mat = newMat;
	this->m_cols = newCols;
	this->m_dist = 1;
}

template<typename T>
VecView<typename MatView<T>::value_type> MatView<T>::row(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) noexcept {
	return VecView<value_type>(
		this->addr(at, from),
		len == 0 ? this->m_cols - from : len
	);
}

template<typename T>
VecView<typename MatView<T>::value_type> MatView<T>::col(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) noexcept {
	return VecView<value_type>(
		this->addr(from, at), 
		len == 0 ? this->m_rows - from : len, 
		this->m_cols
	);
}

template<typename T>
const VecView<typename MatView<T>::value_type> MatView<T>::row(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) const noexcept {
	return VecView<value_type>(
		this->addr(at, from), 
		len == 0 ? this->m_cols - from : len
	);
}

template<typename T>
const VecView<typename MatView<T>::value_type> MatView<T>::col(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) const noexcept {
	return VecView<value_type>(
		this->addr(from, at), 
		len == 0 ? this->m_rows - from : len, 
		this->m_cols
	);
}


template<typename T>
VecView<typename MatView<T>::value_type> MatView<T>::row_safe(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) {
	if (at >= this->m_rows)
		throw std::out_of_range("ca::Mat: row VecView index out of range");
	if (from >= this->m_cols)
		throw std::out_of_range("ca::Mat: row VecView offset out of range");
	if (len + from >= this->m_cols)
		throw std::out_of_range("ca::Mat: row VecView length out of range");
	return row(at, from, len);
}

template<typename T>
VecView<typename MatView<T>::value_type> MatView<T>::col_safe(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) {
	if (at >= this->m_cols)
		throw std::out_of_range("ca::Mat: column VecView index out of range");
	if (from >= this->m_rows)
		throw std::out_of_range("ca::Mat: column VecView offset out of range");
	if (len + from >= this->m_rows)
		throw std::out_of_range("ca::Mat: column VecView length out of range");
	return col(at, from, len);
}

template<typename T>
const VecView<typename MatView<T>::value_type> MatView<T>::row_safe(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) const {
	if (at >= this->m_rows)
		throw std::out_of_range("ca::Mat: row VecView index out of range");
	if (from >= this->m_cols)
		throw std::out_of_range("ca::Mat: row VecView offset out of range");
	if (len + from >= this->m_cols)
		throw std::out_of_range("ca::Mat: row VecView length out of range");
	return row(at, from, len);
}

template<typename T>
const VecView<typename MatView<T>::value_type> MatView<T>::col_safe(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) const {
	if (at >= this->m_cols)
		throw std::out_of_range("ca::Mat: column VecView index out of range");
	if (from >= this->m_rows)
		throw std::out_of_range("ca::Mat: column VecView offset out of range");
	if (len + from >= this->m_rows)
		throw std::out_of_range("ca::Mat: column VecView length out of range");
	return col(at, from, len);
}

template<typename T>
MatView<typename MatView<T>::value_type> MatView<T>::mat(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) noexcept {
	return MatView<value_type>(
		addr(row, col),
		rows, cols,
		m_cols - cols + 1
	);
}

template<typename T>
const MatView<typename MatView<T>::value_type> MatView<T>::mat(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) const noexcept {
	return MatView<value_type>(
		addr(row, col),
		rows, cols,
		m_cols - cols + 1
	);
}

template<typename T>
const MatView<typename MatView<T>::value_type> MatView<T>::mat_safe(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) const noexcept {
	if (col >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView index out of range");
	if (row >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView index out of range");
	if (col + cols >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView offset out of range");
	if (row + rows >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView offset out of range");
	mat(row, col, rows, cols);
}

template<typename T>
MatView<typename MatView<T>::value_type> MatView<T>::mat_safe(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) noexcept {
	if (col >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView index out of range");
	if (row >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView index out of range");
	if (col + cols >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView offset out of range");
	if (row + rows >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView offset out of range");
	mat(row, col, rows, cols);
}

template<typename T>
Mat<T>::~Mat() {
	delete[] this->m_mat;
}

}

#undef CALGO_MAT_INDEX
