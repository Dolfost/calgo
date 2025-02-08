#ifndef _CALGO_STATISTICS_VEC_HPP_
#define _CALGO_STATISTICS_VEC_HPP_

#include <calgo/vec.hpp>
#include <calgo/ss/statistic.hpp>

#define MAP_STAT(NAME, TYPE, ...) CALGO_SS_MAP_STAT(NAME, vec_view<value_type>, TYPE __VA_OPT__(,) __VA_ARGS__)
#define SINGLE_STAT(NAME, TYPE) CALGO_SS_SINGLE_STAT(NAME, vec_view<value_type>, TYPE)

namespace ca::ss {

/**
 * @brief Same as ca::vec_view but with statistical caontainers
 *
 * @note Element acess operations will not clear statistics for speed reasons.
 * If you changed some element with ca::vec_view access operators, then call
 * revoke() to invalidate all statistics.
 *
 * @tparam T `value_type`
 */
template<typename T>
class vec_view: public virtual ::ca::vec_view<T> {
public:
	using typename ::ca::vec_view<T>::size_type;
	using typename ::ca::vec_view<T>::value_type;

public:
	using ::ca::vec_view<T>::vec_view; // ctor
	void revoke();
	
public:
	/// @brief Raw moment of \f(n\f)-th degree \f[ m_i = \sum_{i=1}^N\frac{x_i^n}{N} \f]
	MAP_STAT(raw_moment, value_type);
	value_type mean() { return raw_moment(1); };

	/**
	 * @brief Central moment of \f(n\f)-th degree 
	 * \f[ \hat c_i = \frac{1}{N}\sum_{i=1}^N(x_i - \bar x)^n \text{ - biased} \f]
	 * \f[ c_i = \frac{1}{N-1}\sum_{i=1}^N(x_i - \bar x)^n \text{ - unbiased} \f]
	 */
	MAP_STAT(central_moment, bias<value_type>, value_type);
	bias<value_type> variance() { return central_moment(2); };

	/**
	 * @brief Standard deviation
	 * \f[ \hat \sigma = \sqrt{\frac{1}{N}\sum_{i=1}^N(x_i - \bar x)^2} \text{ - biased} \f]
	 * \f[ \sigma = \sqrt{\frac{1}{N-1}\sum_{i=1}^N(x_i - \bar x)^2} \text{ - unbiased} \f]
	 */
	SINGLE_STAT(std_dev, bias<value_type>);

	/**
	 * @brief Skewness (also known as Assymetry)
	 * \f[ \hat A = \frac{1}{N\hat\sigma^3}\sum_{i=1}^N(x_i-\bar x)^3\text{ - biased} \f]
	 * \f[ \bar A = \frac{\sqrt{N(N-1)}}{N-2}\hat A \text{ - unbiased} \f]
	 */
	SINGLE_STAT(skew, bias<value_type>);
	/**
	 * @brief Kurtosis (also known as Excess)
	 * \f[ \hat E = \frac{\hat\mu_4}{\hat\sigma^4}\text{ - biased} \f]
	 * \f[ \bar E = \frac{N^2-1}{(N-2)(N-3)}\left(\hat E-3)+\frac{6}{N+1}\right)\text{ - unbiased} \f]
	 */
	SINGLE_STAT(kurtosis, bias<value_type>);

};




/**
 * @brief Same as ca::vec but with statistical caontainers
 *
 * Size changind operations invalidate all statistics
 *
 * @tparam T `value_type`
 */
template<typename T>
class vec: public ::ca::vec<T>, public vec_view<T> {
public:
	using typename vec_view<T>::size_type;
	using typename vec_view<T>::value_type;

public:
	/** @name ca::vec and ca::vec_view based constructors
	* @{
	*/
	vec(const size_type& len): ::ca::vec_view<T>(new value_type[len], len) {};
	vec(const ::ca::vec<value_type>& other): 
		::ca::vec_view<value_type>(new value_type[other.n()], other.n()), 
		::ca::vec<value_type>(static_cast<const ::ca::vec_view<value_type>&>(other)) {};
	vec(const ::ca::vec_view<value_type>& other):
		::ca::vec_view<value_type>(new value_type[other.n()], other.n()),
		::ca::vec<value_type>(other) {};
	vec(::ca::vec_view<value_type>&& other): 
	::ca::vec<value_type>(other) {};
	vec(::ca::vec<value_type>&& other): 
		::ca::vec<value_type>(static_cast<::ca::vec_view<value_type>&&>(other)) {};
	vec(std::initializer_list<value_type> data = {}): 
	::ca::vec_view<value_type>(new value_type[data.size()], data.size()),
	::ca::vec<value_type>(data) {};
	/// @}
};

}

#include "../../../src/vec_moments.inl"
#include "../../../src/vec_revoke.inl"

#undef SINGLE_STAT
#undef MAP_STAT

#endif // !_CALGO_STATISTICS_VEC_HPP_
