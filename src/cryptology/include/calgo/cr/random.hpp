#ifndef _CALGO_CRYPTOLOGY_RANDOM_HPP_
#define _CALGO_CRYPTOLOGY_RANDOM_HPP_

#include <calgo/interval.hpp>
#include <calgo/prime.hpp>

#include <stdexcept>
#include <limits>
#include <cstdint>

namespace ca::cr {

/**
 * @brief Base class for random rumbers generators
 *
 * @tparam T `value_type`
 */
template<typename T>
class random {
public:
	using value_type = T;

public:
	/**
	 * @brief Generate random nmber
	 * @return random number
	 */
	virtual value_type generate() = 0;

	/**
	 * @brief Set seed
	 *
	 * Default seed in child classes is 42
	 *
	 * @param seed seed value
	 */
	const value_type& seed() const { return m_seed; }
	const value_type& previous() const { return m_previous; };
	virtual void reset() { m_previous = m_seed; };

protected:
	value_type m_seed, m_previous;
};

/**
 * @brief Lehmer RNG
 *
 * @tparam T `value_type`
 */
template<typename T>
class lehmer: public random<T> {
public:
	using typename random<T>::value_type;

	lehmer() {
		set_seed(42);
	}

public:
	const value_type& modulus() const { return m_modulus; };
	const value_type& multiplier() const { return m_multiplier; };
	const value_type& gain() const { return m_gain; };

	/**
	 * @brief Seed the RNG
	 *
	 * @param seed seed \f(0<x_0<m\f)
	 * @param modulus modulum value \f(0<m\f)
	 * @param multiplier multiplier value \f(0 < a<m\f)
	 * @param gain gain value \f(0 \leq c < m\f).
	 */
	void set_seed(
		const value_type& seed, 
		const value_type& multiplier = 16807, 
		const value_type& gain = 0, 
		const value_type& modulus = std::numeric_limits<value_type>::max()
	) { 
		if (modulus <= 0) 
			throw std::out_of_range("ca::cr::lehmer: modulus <= 0"); 
		if (multiplier <= 0) 
			throw std::out_of_range("ca::cr::lehmer: multiplier <= 0"); 
		if (gain < 0) 
			throw std::out_of_range("ca::cr::lehmer: gain < 0"); 
		if (multiplier >= modulus) 
			throw std::out_of_range("ca::cr::lehmer: multiplier >= modulus"); 
		if (gain >= modulus) 
			throw std::out_of_range("ca::cr::lehmer: gain >= modulus"); 

		m_multiplier = multiplier;
		m_modulus = modulus;
		m_gain = gain;
		this->m_seed = seed;
		this->m_previous = seed;
	};

	value_type generate() override {
		return (this->m_previous = (m_multiplier*this->m_previous + m_gain) % m_modulus);
	};

protected:
	value_type m_modulus, m_multiplier, m_gain;
};

/**
 * @brief BBS RNG implementation
 *
 * @tparam T `value_type`
 */
template<typename T>
class bbs: public random<T> {
public:
	using typename random<T>::value_type;

	bbs() {
		set_seed(42);
	}

public:
	value_type generate() override {
		value_type res = 0;
		for (std::size_t i = 0; i < 8*sizeof(value_type); i++)
			res |= (static_cast<value_type>(generate_bit()) << i);
		return res;
	};

	/**
	 * @brief Generate random bit
	 * @return random bit
	 */
	inline bool generate_bit() {
		return (this->m_previous = (this->m_previous*this->m_previous) % m_n) % 2;
	}

	/**
	 * @brief Seed the RNG
	 *
	 * @param seed seed value
	 * @param p prime \f(p\f) parameter
	 * @param q prime \f(q\f) parameter
	 * @throw std::runtime_error
	 */
	void set_seed(
		const value_type& seed, 
		const value_type& p = 7919, 
		const value_type& q = 6367
	) { 
		if ((p % 4) != 3)
			throw std::runtime_error("ca::cr::bbs: (p mod 4) != 3");
		if ((q % 4) != 3)
			throw std::runtime_error("ca::cr::bbs: (q mod 4) != 3");
		if (not ca::is_prime(p))
			throw std::runtime_error("ca::cr::bbs: p is not a prime");
		if (not ca::is_prime(q))
			throw std::runtime_error("ca::cr::bbs: q is not a prime");
		this->m_seed = seed, m_n = p*q, m_p = p, m_q = q;
		this->m_previous = (seed*seed) % m_n;
	}

	const value_type& p() { return m_p; };
	const value_type& q() { return m_q; };

protected:
	value_type m_p, m_q, m_n;
};


namespace FIPS_140_2 {

static const std::size_t sample_size = 2500;

struct result {
	bool passed = false;
};

struct monobit_result: public result {
	std::size_t ones = 0;
	constexpr static const ca::interval<std::size_t> range = {
		9654,
		10346
	};
};
monobit_result monobit_test(const std::uint8_t* data);

struct poker_result: public result {
	constexpr static const ca::interval<double> range = {2.16, 46.17};
	double statistic = 0;
};
poker_result poker_test(const std::uint8_t* data);

struct long_run_result: public result {
	static const std::size_t max_length = 26;
	std::size_t length = 0;
};
long_run_result long_run_test(const std::uint8_t* data);

struct runs_result: public result {
	constexpr static const ca::interval<std::size_t> ranges[] = {
		{2315, 2685}, 
		{1114, 1386}, 
		{527, 723}, 
		{240, 384}, 
		{103, 209}, 
		{103, 209}, 
	}; /// Expected counts for 2, 3, 4, 5 and 6 bit series
	std::size_t quantities[6][2] = {0};
};
runs_result runs_test(const std::uint8_t* data);

}

}

#endif // !_CALGO_CRYPTOLOGY_RANDOM_HPP_
