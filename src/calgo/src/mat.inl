#pragma once
#include <calgo/mat.hpp>

#include <stdexcept>
#include <algorithm>

#define CALGO_MAT_INDEX(mat, cols, dist, row, col) ((mat) + (row)*((cols)+(dist)-1) + (col))

namespace ca {

template<typename T>
mat_view<T>::mat_view(
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
mat<T>::mat(
	std::initializer_list<std::initializer_list<value_type>> data
): mat_view<T>(nullptr, 0, 0) {
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
mat<T>::mat(
	const size_type& rows, const size_type& cols
): mat_view<T>(new value_type[rows*cols], rows, cols) {
}

template<typename T>
mat<T>::mat(
	const size_type& rows, 
	const size_type& cols, 
	const value_type& init
): mat(rows, cols) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = init;
}

template<typename T>
mat<T>::mat(const mat_view<value_type>& other): mat(other.m_rows, other.m_cols) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = other.el(i, j);
}

template<typename T>
template<typename V>
mat<T>::mat(const mat_view<V>& other): mat(other.rows(), other.cols()) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = static_cast<value_type>(other.el(i, j));
}

template<typename T>
mat<T>::mat(mat<value_type>&& other): mat(other.m_mat, other.m_rows, other.m_cols, other.m_dist) {
	other.m_mat = nullptr;
	other.m_rows = 0;
	other.m_cols = 0;
	other.m_dist = 1;
}

template<typename T>
mat<T>& mat<T>::operator=(const mat_view<value_type>& other) {
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
mat<T>& mat<T>::operator=(mat_view<value_type>&& other) {
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
void mat_view<T>::set(const value_type& val) {
	for (size_type i = 0; i < this->m_rows; i++)
		for (size_type j = 0; j < this->m_cols; j++)
			 this->el(i, j) = val;
}

template<typename T>
void mat<T>::resize(const size_type& rows, const size_type& cols, bool copy) {
	if (rows == this->m_rows and cols == this->m_cols)
		return;

	size_type size = rows*cols;
	if (copy) {
		value_type* newMat = new value_type[size];
		size_type stopRow = std::min(rows, this->m_rows);
		size_type stopCol = std::min(cols, this->m_cols);
		for (size_type i = 0; i < stopRow; i++)
			for (size_type j = 0; j < stopCol; j++)
				*CALGO_MAT_INDEX(newMat, cols, 1, i, j) = 
					this->el(i, j);
		for (size_type i = 0; i < rows - stopRow; i++)
			for (size_type j = 0; j < cols - stopCol; j++)
				*CALGO_MAT_INDEX(newMat, cols, 1, i, j) = 0;
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
vec_view<typename mat_view<T>::value_type> mat_view<T>::operator[](const size_type& row) noexcept {
	return vec_view<value_type>(this->addr(row, 0), this->m_cols);
}

template<typename T>
const vec_view<T> mat_view<T>::operator[](const size_type& row) const noexcept {
	return vec_view<value_type>(this->addr(row, 0), this->m_cols);
}

template<typename T>
typename mat_view<T>::value_type* mat_view<T>::addr(const size_type& row, const size_type& col) const noexcept {
	return CALGO_MAT_INDEX(m_mat, m_cols, m_dist, row, col);
}

template<typename T>
typename mat_view<T>::value_type& mat_view<T>::el(const size_type& row, const size_type& col) noexcept {
	return *addr(row, col);
}

template<typename T>
const typename mat_view<T>::value_type& mat_view<T>::el(const size_type& row, const size_type& col) const noexcept {
	return *addr(row, col);
}

template<typename T>
typename mat_view<T>::value_type& mat_view<T>::operator()(const size_type& row, const size_type& col) noexcept {
	return el(row, col);
}

template<typename T>
const typename mat_view<T>::value_type& mat_view<T>::operator()(const size_type& row, const size_type& col) const noexcept {
	return el(row, col);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::at(const size_type& row) {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	return operator[](row);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::at(const size_type& row) const {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	return operator[](row);
}

template<typename T>
const typename mat_view<T>::value_type& mat_view<T>::at(const size_type& row, const size_type& col) const {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	if (col >= this->m_cols)
		throw std::out_of_range("ca::Mat column out of range");
	return el(row, col);
}

template<typename T>
typename mat_view<T>::value_type& mat_view<T>::at(const size_type& row, const size_type& col) {
	if (row >= this->m_rows)
		throw std::out_of_range("ca::Mat row out of range");
	if (col >= this->m_cols)
		throw std::out_of_range("ca::Mat column out of range");
	return el(row, col);
}

template<typename T>
template<typename D>
std::ostream& mat_view<T>::showSystem(const vec_view<D>& v, std::ostream& os) const {
	if (this->m_rows != v.m_len)
		throw std::runtime_error("ca::Mat: can not print system");

	for (size_type i = 0; i < v.m_len; i++)
			os << row(i) << "\t | " << v[i] << '\n';
	return os;
}

template<typename T>
std::ostream& mat_view<T>::as_array(std::ostream& os) const {
	for (size_type i = 0; i < m_rows; i++) {
		row(i).asArray(os); 
		os << ",\n";
	}
	return os;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const mat_view<D>& m) {
	for (typename mat_view<D>::size_type i = 0; i < m.m_rows; i++)
		os << m[i] << '\n';
	return os;
}

template<typename T>
void ca::mat<T>::insert_rows(const size_type& row, const size_type& count, const value_type& init) {
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
void ca::mat<T>::remove_rows(const size_type& row, const size_type& count) {
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
void ca::mat<T>::insert_cols(const size_type& col, const size_type& count, const value_type& init) {
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
void ca::mat<T>::remove_cols(const size_type& col, const size_type& count) {
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
template<typename rhs_typename>
bool mat_view<T>::operator==(const mat_view<rhs_typename>& other) const noexcept {
	if (m_rows != other.rows() or m_cols != other.cols())
		return false;
	for (size_type i = 0; i < m_rows; i++)
		for (size_type j = 0; j < m_cols; j++)
			if (el(i, j) != other.el(i, j))
				return false;
	return true;
}

template<typename T>
template<typename rhs_value_type, class comparator>
typename std::enable_if<std::is_invocable_r<bool, comparator, typename mat_view<T>::value_type, rhs_value_type>::value, bool>::type 
mat_view<T>::compare(const mat_view<rhs_value_type>& other, const comparator& comp) const noexcept {
	if (m_rows != other.rows() or m_cols != other.cols())
		return false;
	for (size_type i = 0; i < m_rows; i++)
		for (size_type j = 0; j < m_cols; j++)
			if (not comp(el(i, j), other.el(i, j)))
				return false;
	return true;
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::row(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) noexcept {
	return vec_view<value_type>(
		this->addr(at, from),
		len == 0 ? this->m_cols - from : len
	);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::col(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) noexcept {
	return vec_view<value_type>(
		this->addr(from, at), 
		len == 0 ? this->m_rows - from : len, 
		this->m_cols
	);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::row(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) const noexcept {
	return vec_view<value_type>(
		this->addr(at, from), 
		len == 0 ? this->m_cols - from : len
	);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::col(
	const size_type& at, 
	const size_type& from, 
	const size_type& len
) const noexcept {
	return vec_view<value_type>(
		this->addr(from, at), 
		len == 0 ? this->m_rows - from : len, 
		this->m_cols
	);
}


template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::row_safe(
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
vec_view<typename mat_view<T>::value_type> mat_view<T>::col_safe(
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
const vec_view<typename mat_view<T>::value_type> mat_view<T>::row_safe(
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
const vec_view<typename mat_view<T>::value_type> mat_view<T>::col_safe(
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
vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal(const size_type& index, diagonal_type dt) noexcept {
	if (dt == diagonal_type::upper)
		return diagonal_upper(index);
	return diagonal_lower(index);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal(const size_type& index, diagonal_type dt) const noexcept {
	if (dt == diagonal_type::upper)
		return diagonal_upper(index);
	return diagonal_lower(index);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_safe(const size_type& index, diagonal_type dt) const {
	if (dt == diagonal_type::upper)
		return diagonal_upper_safe(index);
	return diagonal_lower_safe(index);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_safe(const size_type& index, diagonal_type dt) {
	if (dt == diagonal_type::upper)
		return diagonal_upper_safe(index);
	return diagonal_lower_safe(index);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_upper(const size_type& index) noexcept {
	return vec_view<value_type>(addr(0, index), m_rows - index, m_cols + 1);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_lower(const size_type& index) const noexcept {
	return vec_view<value_type>(addr(index, 0), m_rows - index, m_cols + 1);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_upper_safe(const size_type& index) {
	if (index >= m_cols)
		throw std::out_of_range("ca::Mat: diagonal index is too large");
	return diagonal_upper(index);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_upper_safe(const size_type& index) const {
	if (index >= m_cols)
		throw std::out_of_range("ca::Mat: diagonal index is too large");
	return diagonal_upper(index);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_lower_safe(const size_type& index) const {
	if (index >= m_rows)
		throw std::out_of_range("ca::Mat: diagonal index is too large");
	return diagonal_lower(index);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal_lower_safe(const size_type& index) {
	if (index >= m_rows)
		throw std::out_of_range("ca::Mat: diagonal index is too large");
	return diagonal_lower(index);
}

template<typename T>
vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal() noexcept {
	return diagonal_upper(0);
}

template<typename T>
const vec_view<typename mat_view<T>::value_type> mat_view<T>::diagonal() const noexcept {
	return diagonal_upper(0);
}

template<typename T>
mat_view<typename mat_view<T>::value_type> mat_view<T>::submat(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) noexcept {
	return mat_view<value_type>(
		addr(row, col),
		rows, cols,
		m_cols - cols + 1
	);
}

template<typename T>
const mat_view<typename mat_view<T>::value_type> mat_view<T>::submat(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) const noexcept {
	return mat_view<value_type>(
		addr(row, col),
		rows, cols,
		m_cols - cols + 1
	);
}

template<typename T>
const mat_view<typename mat_view<T>::value_type> mat_view<T>::submat_safe(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) const {
	if (col >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView index out of range");
	if (row >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView index out of range");
	if (col + cols >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView offset out of range");
	if (row + rows >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView offset out of range");
	submat(row, col, rows, cols);
}

template<typename T>
mat_view<typename mat_view<T>::value_type> mat_view<T>::submmat_safe(
	const size_type& row,
	const size_type& col,
	const size_type& rows,
	const size_type& cols
) {
	if (col >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView index out of range");
	if (row >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView index out of range");
	if (col + cols >= m_cols)
		throw std::out_of_range("ca::Mat: column MatView offset out of range");
	if (row + rows >= m_rows)
		throw std::out_of_range("ca::Mat: row MatView offset out of range");
	submat(row, col, rows, cols);
}

template<typename T>
template<typename rhs_value_type> 
auto mat_view<T>::mul(const ca::mat_view<rhs_value_type>& b) const noexcept -> ca::mat<typename product<rhs_value_type>::type> {
	mat<typename product<rhs_value_type>::type> prod(m_rows, b.cols(), 0);
	for (size_type i = 0; i < m_rows; i++)
		for (size_type j = 0; j < b.cols(); j++)
			for (size_type k = 0; k < m_cols; k++) 
				prod.el(i, j) += el(i, k) * b.el(k, j);
	return prod;
}

template<typename T>
template<typename rhs_value_type> 
auto mat_view<T>::mul(const ca::vec_view<rhs_value_type>& b) const noexcept -> ca::vec<typename product<rhs_value_type>::type> {
	vec<typename product<rhs_value_type>::type> prod(m_rows, 0);
	for (size_type i = 0; i < m_rows; i++)
		for (size_type k = 0; k < m_cols; k++) 
			prod.el(i) += el(i, k) * b.el(k);
	return prod;
}

template<typename T>
template<typename rhs_value_type> 
auto mat_view<T>::mul_safe(const ca::mat_view<rhs_value_type>& b) const -> ca::mat<typename product<rhs_value_type>::type> {
	if (not is_product_comformable(b))
		throw std::logic_error("ca::Mat: can not multiply non-conformant matrices");
	return mul(b);
}

template<typename T>
template<typename rhs_value_type> 
auto mat_view<T>::mul_safe(const ca::vec_view<rhs_value_type>& b) const -> ca::vec<typename product<rhs_value_type>::type> {
	if (not is_product_comformable(b))
		throw std::logic_error("ca::Mat: can not multiply non-conformant matrice and vector");
	return mul(b);
}

template<typename T>
template<typename rad_type> 
void mat_view<T>::rotation2d(const rad_type& rad) noexcept {
	el(0, 0) = std::cos(rad);
	el(0, 1) = -std::sin(rad);
	el(1, 0) = -el(0, 1);
	el(1, 1) = el(0, 0);
}

template<typename T>
template<typename V>
typename std::enable_if<std::is_arithmetic<V>::value, V>::type
mat_view<T>::det() const noexcept {
	mat<V> triangular = *this;
	signed char sign = 1;
	for (size_type col = 0; col < m_cols - 1; ++col) {
		// pivot row with largest absolute value in this column
		size_type pivot = col;
		V max_val = std::abs(triangular.el(col, col));
		for (size_type r = col + 1; r < m_rows; ++r) {
			V val = std::abs(triangular.el(r, col));
			if (val > max_val) {
				max_val = val;
				pivot = r;
			}
		}
		// if pivot element is zero ==> determinant = 0
		if (max_val == 0)
			return 0;
		// swap rows if needed and flip determinant sign
		if (pivot != col) {
			for (size_type c = 0; c < m_cols; ++c)
				std::swap(triangular.el(col, c), triangular.el(pivot, c));
			sign = -sign;
		}
		// eliminate entries below the pivot
		for (size_type row = col + 1; row < m_rows; ++row) {
			V factor = triangular.el(row, col) / triangular.el(col, col);
			for (size_type i = col; i < m_cols; ++i)
				triangular.el(row, i) -= factor * triangular.el(col, i);
		}
	}
	return sign * triangular.diagonal().prod();
}

template<typename T>
template<typename V>
typename std::enable_if<std::is_arithmetic<V>::value, V>::type
mat_view<T>::det_safe() const {
	if (not is_square())
		throw std::logic_error("ca::Mat: can not calculate determinant of non-square matrix");
	return det<V>();
}

template<typename T>
template<typename V>
typename std::enable_if<std::is_arithmetic<V>::value, mat<V>>::type
mat_view<T>::inverse() const {
	mat<V> augmented(m_rows, 2*m_rows, 0);
	for (size_type i = 0; i < m_rows; ++i) {
		for (size_type j = 0; j < m_rows; ++j)
			augmented.el(i, j) = static_cast<V>(this->el(i, j));
		augmented.el(i, m_rows + i) = 1; // identity part
	}

	// perform Gaussian elimination with partial pivoting
	for (size_type col = 0; col < m_rows; ++col) {
		size_type pivot = col;
		V max_val = std::abs(augmented.el(col, col));
		for (size_type r = col + 1; r < m_rows; ++r) {
			V val = std::abs(augmented.el(r, col));
			if (val > max_val) {
				max_val = val;
				pivot = r;
			}
		}

		// singular check
		if (max_val == 0)
			throw std::runtime_error("ca::Mat: can not find inverse of an singular matrix");

		// swap rows if needed
		if (pivot != col)
			for (size_type c = 0; c < 2*m_rows; ++c)
				std::swap(augmented.el(col, c), augmented.el(pivot, c));
		// normalize pivot row
		V pivot_val = augmented.el(col, col);
		for (size_type c = 0; c < 2*m_rows; ++c)
			augmented.el(col, c) /= pivot_val;
		// eliminate all other rows
		for (size_type r = 0; r < m_rows; ++r) {
			if (r == col) continue;
			V factor = augmented.el(r, col);
			for (size_type c = 0; c < 2*m_rows; ++c)
				augmented.el(r, c) -= factor * augmented.el(col, c);
		}
	}
	return augmented.submat(0, m_cols, m_rows, m_cols);
}

template<typename T>
mat<T> mat_view<T>::transpose() const noexcept {
	mat<value_type> m(m_cols, m_rows);
	for (size_type i = 0; i < m_rows; i++)
		for (size_type j = 0; j < m_cols; j++)
			m.el(j, i) = el(i, j);
	return m;
}

template<typename T>
template<typename V>
typename std::enable_if<std::is_arithmetic<V>::value, mat<V>>::type
mat_view<T>::inverse_safe() const {
	if (not is_square()) 
		throw std::logic_error("ca::Mat: can not find inverse of non-square matrix");
	return inverse<V>();
}

template<typename T>
mat<T>::~mat() {
	delete[] this->m_mat;
}

}

#undef CALGO_MAT_INDEX
