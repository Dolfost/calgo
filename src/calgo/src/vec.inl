#pragma once
#include <calgo/vec.hpp>

#include <stdexcept>
#include <iomanip>
#include <algorithm>

namespace ca {

template<typename T>
Vec<T>::Vec(
	std::initializer_list<value_type> data
): VecView<T>(new value_type[data.size()], data.size()) {
	size_type i = 0;
	for (const auto& elem: data) {
		this->m_vec[i++] = elem;
	}
}

template<typename T>
Vec<T>::Vec(
	const size_type& len
): VecView<T>(new value_type[len], len) {}

template<typename T>
Vec<T>::Vec(const Vec<value_type>& other): Vec(other.m_len) {
	for (size_type i = 0; i < this->m_len; i++)
		this->m_vec[i] = other[i];
}

template<typename T>
Vec<T>& Vec<T>::operator=(const Vec<T>& other) {
	if (other.m_len != this->m_len) {
		delete[] this->m_vec;
		this->m_len = other.m_len;
		this->m_vec = new value_type[this->m_len];
	}
	for (size_type i = 0; i < this->m_len; i++)
		this->m_vec[i] = other[i];
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
			newVec[i] = this->m_vec[i];
		for (size_type i = stop; i < len - stop; i++)
			newVec[i] = 0;
		delete[] this->m_vec;
		this->m_vec = newVec;
	} else {
		delete[] this->m_vec;
		this->m_vec = new value_type[len];
	}
	this->m_len = len;
}

template<typename T>
typename VecView<T>::size_type VecView<T>::n() const {
	return m_len;
}

template<typename T>
VecView<T>::VecView(value_type* data, size_type len) {
	this->m_vec = data; 
	this->m_len = len;
}


template<typename T>
const typename VecView<T>::value_type& VecView<T>::operator[](
	const size_type& index
) const noexcept {
	return *(m_vec + index);
}

template<typename T>
typename VecView<T>::value_type& VecView<T>::operator[](
	const size_type& index
) noexcept {
	return *(m_vec + index);
}

template<typename T>
const typename VecView<T>::value_type& VecView<T>::at(
	const size_type& index
) const {
	if (index >= m_len)
		throw std::out_of_range("ca::Vector index is out of range");
	return operator[](index);
}

template<typename T>
typename VecView<T>::value_type& VecView<T>::at(
	const size_type& index
) {
	if (index >= m_len)
		throw std::out_of_range("ca::Vector index is out of range");
	return operator[](index);
}

template<typename T>
template<typename M>
void VecView<T>::operator*=(const M& d) noexcept {
	static_assert(std::is_arithmetic<M>(), "Not an arithmetic type");
	for (size_type i = 0; i < m_len; i++)
		m_vec[i] *= d;
}

template<typename T>
typename VecView<T>::value_type VecView<T>::dot(const VecView<value_type>& other) {
	if (m_len != other.m_len)
		throw std::runtime_error("ca::Matrix: dot product not possible");
	operator*(other);
}

template<typename T>
typename VecView<T>::value_type VecView<T>::operator*(const VecView<value_type>& other) noexcept {
	value_type prod = 0;
	for (size_type i = 0; i < m_len; i++)
		prod += this->at(i)*other[i];

	return prod;
}

template<typename D>
std::ostream& operator<<(std::ostream& os, const VecView<D>& v) {
	for (long long i = 0; i < (long long)v.m_len - 1; i++)
		os << std::setw(9) << std::fixed << std::setprecision(4) << std::left << v[i];
	if (v.m_len >= 1)
		os << v[v.m_len-1];
	return os;
}

template<typename T>
Vec<T>::~Vec() {
	delete[] this->m_vec;
}

}
