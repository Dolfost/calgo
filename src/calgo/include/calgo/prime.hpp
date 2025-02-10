#ifndef _CALGO_PRIME_HPP_
#define _CALGO_PRIME_HPP_

#include <cstddef>
#include <type_traits>

namespace ca {

/**
 * @brief Check if number is a prime 
 *
 * Input number \f(n\f) should be \f(|n|\leq \sqrt{s}\f) where \f(s=\f)`std::numeric_limits<std::size_t>::max()`.
 *
 * @tparam T integral type
 * @param number number to check
 * @return `true` if `number` is prime, `false` otherwise
 */
template<typename T>
bool is_prime(T number) {
	static_assert(std::is_integral_v<T>, "T should be integral type");
	if(number < 2) 
		return false;
	if(number == 2) 
		return true;
	if(number % 2 == 0) 
		return false;
	for (std::size_t i = 3; (i*i) <= number; i += 2)
		if (number % i == 0 ) 
			return false;
	return true;
}

}

#endif // !_CALGO_PRIME_HPP_
