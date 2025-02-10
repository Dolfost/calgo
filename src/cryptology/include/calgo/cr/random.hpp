#ifndef _CALGO_CRYPTOLOGY_RANDOM_HPP_
#define _CALGO_CRYPTOLOGY_RANDOM_HPP_

#include <calgo/interval.hpp>

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
	 * Default seed is 42
	 *
	 * @param seed seed value
	 */
	virtual void set_seed(const value_type& seed) { m_seed = seed; }
	const value_type& seed() const { return m_seed; }

protected:
	value_type m_seed = 42;
};

/**
 * @brief Lehmer random generator
 *
 * @tparam T `value_type`
 */
template<typename T>
class lehmer: public random<T> {
public:
	using typename random<T>::value_type;
public:
	const value_type& modulus() const { return m_modulus; };
	const value_type& multiplier() const { return m_multiplier; };
	const value_type& gain() const { return m_gain; };
	const value_type& previous() const { return m_previous; };

	/**
	 * @brief Set modulus \f(0 < m\f)
	 *
	 * Default modulus value is `std::numeric_limits<value_type>::max() - 1`
	 *
	 * @param m new modulus value
	 * @throws std::out_of_range
	 */
	void set_modulus(const value_type& m) { 
		if (m <= 0) 
			throw std::out_of_range("ca::cr::lehmer: modulus is 0"); 
		m_modulus = m; 
	};
	/**
	 * @brief Set multiplier \f(0 < a < m\f)
	 *
	 * Default multiplier value is 16807.
	 *
	 * @param a new multiplier value
	 * @throws std::out_of_range
	 */
	void set_multiplier(const value_type& a) { 
		if (a >= m_modulus) 
			throw std::out_of_range("ca::cr::lehmer: modulus > multiplier"); 
		if (a <= 0) 
			throw std::out_of_range("ca::cr::lehmer: multiplier is 0"); 
		m_multiplier = a;
	};
	/**
	 * @brief Set gain value \f(0 \leq c < m\f)
	 *
	 * Default gain value is 0.
	 *
	 * @param c new gain value
	 * @throws std::out_of_range
	 */
	void set_gain(const value_type& c) {
		if (c >= m_modulus) 
			throw std::out_of_range("ca::cr::lehmer: modulus > gain"); 
		m_gain = c;
	};
	void set_previous(const value_type& prev) {
		m_previous = prev;
	};

	/**
	 * @brief Set the seed
	 * Also the m_previous is beeing set to `seed`
	 * @param seed new seed value
	 */
	void set_seed(const value_type& seed) override {
		random<T>::set_seed(seed);
		set_previous(m_previous);
	}

	value_type generate() override {
		return (m_previous = (m_multiplier*m_previous + m_gain) % m_modulus);
	};

protected:
	value_type m_modulus = std::numeric_limits<value_type>::max() - 1;
	value_type m_multiplier = 16807, m_gain = 0, m_previous = this->m_seed; 
};

namespace FIBS_140_1 {

static const std::size_t sample_size = 2500;

struct result {
	bool passed = false;
};

struct monobit_result: public result {
	std::size_t ones = 0;
	static const std::size_t ones_from = 9654;
	static const std::size_t ones_to = 10346;
};
monobit_result monobit_test(const std::uint8_t* data);

struct poker_result: public result {
	constexpr static const ca::interval<double> range = {1.03, 57.4};
	double statistic = 0;
};
poker_result poker_test(const std::uint8_t* data);

struct series_length_result: public result {
	static const std::size_t max_length = 34;
	std::size_t length = 0;
};
series_length_result series_length_test(const std::uint8_t* data);

struct series_result: public result {
	constexpr static const ca::interval<std::size_t> ranges[] = {
		{1114, 1386}, 
		{527, 723}, 
		{240, 384}, 
		{103, 209}, 
		{103, 209}, 
	}; /// Expected counts for 2, 3, 4, 5 and 6 bit series
	std::size_t quantities[5] = {0};
};
series_result series_test(const std::uint8_t* data);

}

}

#endif // !_CALGO_CRYPTOLOGY_RANDOM_HPP_
