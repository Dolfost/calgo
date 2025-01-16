#pragma once
#include <calgo/vec.hpp>

#include <stdexcept>
#include <iomanip>
#include <algorithm>

#define CALGO_VEC_INDEX(vec, dist, index) (vec + (dist)*(index))

namespace ca {

template<typename T>
Vec<T>::Vec(
	std::initializer_list<value_type> data
): VecView<T>(new value_type[data.size()], data.size()) {
	size_type i = 0;
	for (const auto& elem: data) {
		this->el(i++) = elem;
	}
}

template<typename T>
Vec<T>::Vec(
	const size_type& len
): VecView<T>(new value_type[len], len) {}

template<typename T>
Vec<T>::Vec(const Vec<value_type>& other): Vec(other.m_len) {
	for (size_type i = 0; i < this->m_len; i++)
		this->el(i) = other[i];
}
template<typename T>
Vec<T>::Vec(const VecView<value_type>& other): Vec(other.m_len) {
	for (size_type i = 0; i < this->m_len; i++)
		this->el(i) = other[i];
}

template<typename T>
Vec<T>& Vec<T>::operator=(const Vec<T>& other) {
	if (other.m_len != this->m_len) {
		delete[] this->m_vec;
		this->m_len = other.m_len;
		this->m_vec = new value_type[this->m_len];
	}
	for (size_type i = 0; i < this->m_len; i++)
		this->el(i) = other[i];
	return *this;
}

template<typename T>
void Vec<T>::resize(const size_type& len, bool copy) {
	if (len == this->m_len)
		return;

	if (copy) {
		value_type* newVec = new value_type[len];
		size_type stop = std::min(this->m_len, len);
		for (size_type i = 0; i < stop; i++)
			newVec[i] = this->el(i);
		for (size_type i = stop; i < len - stop; i++)
			newVec[i] = 0;
		delete[] this->m_vec;
		this->m_vec = newVec;
	} else {
		delete[] this->m_vec;
		this->m_vec = new value_type[len];
	}
	this->m_len = len;
	this->m_dist = 1;
}

template<typename T>
typename VecView<T>::size_type VecView<T>::n() const {
	return m_len;
}

template<typename T>
void VecView<T>::set(const value_type& val) {
	for (size_type i = 0; i < m_len; i++) {
		el(i) = val;
	}
}

template<typename T>
VecView<T>::VecView(value_type* data, const size_type& len, const size_type& dist) {
	this->m_vec = data; 
	this->m_len = len;
	this->m_dist = dist;
}

template<typename T>
typename VecView<T>::value_type& VecView<T>::el(const size_type& index) noexcept {
	return *CALGO_VEC_INDEX(m_vec, m_dist, index);
}

template<typename T>
const typename VecView<T>::value_type& VecView<T>::el(const size_type& index) const noexcept {
	return *CALGO_VEC_INDEX(m_vec, m_dist, index);
}

template<typename T>
typename VecView<T>::value_type* VecView<T>::addr(const size_type& index) const noexcept {
	return CALGO_VEC_INDEX(m_vec, m_dist, index);
}

template<typename T>
const typename VecView<T>::value_type& VecView<T>::operator[](
	const size_type& index
) const noexcept {
	return el(index);
}

template<typename T>
typename VecView<T>::value_type& VecView<T>::operator[](
	const size_type& index
) noexcept {
	return el(index);
}

template<typename T>
const typename VecView<T>::value_type& VecView<T>::at(
	const size_type& index
) const {
	if (index >= m_len)
		throw std::out_of_range("ca::Vec index is out of range");
	return el(index);
}

template<typename T>
typename VecView<T>::value_type& VecView<T>::at(
	const size_type& index
) {
	if (index >= m_len)
		throw std::out_of_range("ca::Vec index is out of range");
	return el(index);
}

template<typename T>
template<typename M>
void VecView<T>::operator*=(const M& d) noexcept {
	static_assert(std::is_arithmetic<M>(), "Not an arithmetic type");
	for (size_type i = 0; i < m_len; i++)
		el(i) *= d;
}

template<typename T>
typename VecView<T>::value_type VecView<T>::dot(const VecView<value_type>& other) {
	if (m_len != other.m_len)
		throw std::runtime_error("ca::Vec: dot product not possible");
	operator*(other);
}

template<typename T>
typename VecView<T>::value_type VecView<T>::operator*(const VecView<value_type>& other) noexcept {
	value_type prod = 0;
	for (size_type i = 0; i < m_len; i++)
		prod += el(i)*other[i];

	return prod;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const VecView<D>& v) {
	for (long long i = 0; i < (long long)v.m_len - 1; i++)
		os << std::setw(9) << std::fixed << std::setprecision(4) << std::left <<
			v[i];
	if (v.m_len >= 1)
		os << v[v.m_len - 1];
	return os;
}

template<typename T>
void Vec<T>::insert(const size_type& where, const size_type& count, const value_type& init) {
	if (where > this->m_len)
		throw std::out_of_range("ca::Vec: insertion index out of range");

	size_type newLen = this->m_len + count;
	value_type* newVec = new value_type[newLen];

	std::copy(this->m_vec, this->m_vec + where, newVec);
	std::fill(newVec + where, newVec + where + count, init);
	std::copy(this->m_vec + where, this->m_vec + this->m_len, newVec + where + count);

	delete[] this->m_vec;
	this->m_vec = newVec;
	this->m_len = newLen;
}

template<typename T>
void Vec<T>::remove(const size_type& from, const size_type& count) {
	if (from >= this->m_len || from + count > this->m_len)
		throw std::out_of_range("ca::Vec: removal index out of range");

	size_type newLen = this->m_len - count;
	value_type* newVec = new value_type[newLen];

	std::copy(this->m_vec, this->m_vec + from, newVec);
	std::copy(this->m_vec + from + count, this->m_vec + this->m_len, newVec + from);

	delete[] this->m_vec;
	this->m_vec = newVec;
	this->m_len = newLen;
}

template<typename T>
Vec<T>::~Vec() {
	delete[] this->m_vec;
}

}

#undef CALGO_VEC_INDEX
