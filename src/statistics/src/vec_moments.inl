#pragma once
#include <calgo/ss/vec.hpp>

#include <cmath>

namespace ca::ss {

template<typename T>
void vec_view<T>::raw_moment_impl::m_compute(const key_type& degree) {
	value_type& moment = this->m_values[degree];
	moment = 0;

	size_type n = this->m_owner.n();
	if (n == 0)
		return;

	for (size_type i = 0; i < n; i++)
		moment += std::pow(this->m_owner[i], degree);

	moment /= n;
}

template<typename T>
void vec_view<T>::central_moment_impl::m_compute(const key_type& degree) {
	value_type& moment = this->m_values[degree];
	moment.biased = 0;

	size_type n = this->m_owner.n();
	if (n == 0) {
		moment.unbiased = 0;
		return;
	}

	const typename value_type::value_type mean = this->m_owner.raw_moment(1);
	for (size_type i = 0; i < n; i++)
		moment.biased += std::pow(this->m_owner[i] - mean, degree);

	moment.unbiased = moment.biased;
	moment.biased /= n;
	moment.unbiased /= n - 1;
}

template<typename T>
void vec_view<T>::std_dev_impl::m_compute() {
	this->m_value = std::make_optional(value_type());
	this->m_value.value().biased = std::sqrt(this->m_owner.central_moment(2).biased);
	this->m_value.value().unbiased = std::sqrt(this->m_owner.central_moment(2).unbiased);
}

template<typename T>
void vec_view<T>::skew_impl::m_compute() {
	this->m_value = std::make_optional(value_type());
	value_type& skew = this->m_value.value();
  size_type n = this->m_owner.n();

  skew.biased = this->m_owner.central_moment(3).unbiased /
		std::pow(this->m_owner.std_dev().unbiased, 3);
  skew.unbiased =
		(std::sqrt(n * (n - 1)) / (n - 2)) * skew.biased;
}

template<typename T>
void vec_view<T>::kurtosis_impl::m_compute() {
	this->m_value = std::make_optional(value_type());
	value_type& kurtosis = this->m_value.value();
  size_type n = this->m_owner.n();

	kurtosis.biased = this->m_owner.central_moment(4).biased /
		pow(this->m_owner.std_dev().biased, 4);
	kurtosis.unbiased = ((std::pow(n, 2) - 1) / ((n - 2) * (n - 3))) *
		((kurtosis.biased - 3) + 6 / (n + 1));
}

}
