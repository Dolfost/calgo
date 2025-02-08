#ifndef _CALGO_STATISTICS_STAT_HPP_
#define _CALGO_STATISTICS_STAT_HPP_

#include <unordered_map>
#include <optional>

namespace ca::ss {

/**
 * @brief Base class for statistical values
 *
 * This class is intended to be inherited and used as field in `owner_type` class.
 *
 * @tparam Owner `owner_type`
 */
template<class Owner>
class statistic {
public:
	using owner_type = Owner;
	using size_type = typename owner_type::size_type;

public:
	statistic(owner_type& owner): m_owner(owner) {};
	statistic(const statistic<owner_type>& other) {};
	statistic(const statistic<owner_type>&& other) {};

	const owner_type& owner() { return m_owner; }

public:
	virtual void revoke() = 0;

protected:
	owner_type& m_owner;
};

/**
 * @brief Single statistic
 *
 * The std::optional wrapper
 *
 * @tparam Owner `owner_type`
 * @tparam Type `value_type`
 */
template<class Owner, typename Type>
class single_statistic: public statistic<Owner> {
public:
	using value_type = Type;
	using typename statistic<Owner>::owner_type;
	using size_type = typename owner_type::size_type;

public:
	using statistic<Owner>::statistic;
	single_statistic(const single_statistic<owner_type, value_type>& other): statistic<owner_type>(other) {
		m_value = other.m_value;
	}
	single_statistic(const single_statistic<owner_type, value_type>&& other): statistic<owner_type>(other) {
		m_value = std::move(other.m_value);
	}

	const value_type& value() {
		if (not m_value)
			m_compute();
		return m_value.value();
	}

	const value_type& operator()() {
		return value();
	}

	void revoke() override { m_value.reset(); }

protected:
	virtual void m_compute() = 0;

	std::optional<value_type> m_value;
};

/**
 * @brief Multiple statistic (one key)
 *
 * The std::unordered_map wrapper
 *
 * @tparam Owner `owner_type`
 * @tparam Type `value_type`
 * @tparam Key `key_type`
 */
template<class Owner, typename Type, typename Key = Type>
class map_statistic: public statistic<Owner> {
public:
	using value_type = Type;
	using key_type = Key;
	using typename statistic<Owner>::owner_type;
	using typename statistic<Owner>::size_type;

public:
	using statistic<Owner>::statistic;
	map_statistic(const map_statistic<owner_type, value_type>& other): statistic<owner_type>(other) {
		m_values = other.m_values;
	}
	map_statistic(const map_statistic<owner_type, value_type>&& other): statistic<owner_type>(other) {
		m_values = std::move(other.m_values);
	}

	const value_type& value(const key_type& key) {
		if (not m_values.count(key))
			m_compute(key);
		return m_values[key];
	}

	const value_type& operator()(const key_type& key) {
		return value(key);
	}

	void revoke() override { m_values.clear(); }

protected:
	virtual void m_compute(const key_type& key) = 0;

	std::unordered_map<key_type, value_type> m_values;
};

/**
 * @brief Biased/unbiased pair
 *
 * Represents biased and unbiased pair of statistical value
 *
 * @tparam T `value_type`
 */
template<typename T>
struct bias {
	using value_type = T;
	value_type biased = 0;
	value_type unbiased = 0;
};

}

#define CALGO_SS_SINGLE_STAT(NAME, OWNER, TYPE, ...) \
	class NAME##_impl: public ::ca::ss::single_statistic<OWNER, TYPE> { \
	public: \
		using base_type = ::ca::ss::single_statistic<OWNER, TYPE>; \
		using typename base_type::value_type; \
		using typename base_type::owner_type; \
		using typename base_type::size_type; \
	public: \
		using base_type::base_type; \
	protected: \
		void m_compute() override; \
	} NAME = NAME##_impl(*this);

#define CALGO_SS_MAP_STAT(NAME, OWNER, TYPE, ...) \
	class NAME##_impl: public ::ca::ss::map_statistic<OWNER, TYPE __VA_OPT__(,) __VA_ARGS__> { \
	public: \
		using base_type = typename ::ca::ss::map_statistic<OWNER, TYPE __VA_OPT__(,) __VA_ARGS__>; \
		using typename base_type::value_type; \
		using typename base_type::key_type; \
		using typename base_type::owner_type; \
		using typename base_type::size_type; \
	public: \
		using base_type::base_type; \
	protected: \
		void m_compute(const key_type& key) override; \
	} NAME = NAME##_impl(*this);

#endif // !_CALGO_STATISTICS_STAT_HPP_
