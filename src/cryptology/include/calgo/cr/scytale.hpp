#ifndef _CALGO_CRYPTOLOGY_SCYTALE_HPP_
#define _CALGO_CRYPTOLOGY_SCYTALE_HPP_

#include <calgo/cr/cipher.hpp>

#include <calgo/macros.hpp>

#include <string>
#include <type_traits>
#include <list>

namespace ca::cr {

/**
 * @brief Scytale cipher implementation
 */
class scytale: public cipher {
public:
	using cipher::size_type;
	using string_type = std::string;
	using key_type = size_type;

public:
	const void* decrypted_data() const override { return m_decrypted.c_str(); }
	const void* encrypted_data() const override { return m_encrypted.c_str(); }

	size_type decrypted_data_size() const override { return m_decrypted.size(); }
	size_type encrypted_data_size() const override { return m_decrypted.size(); }

	void encrypt() override;
	void decrypt() override;

	const string_type& encrypted_str() const { return m_encrypted; }
	const string_type& decrypted_str() const { return m_decrypted; }

	template<typename P> typename 
	std::enable_if<std::is_assignable<string_type, P>::value>::type set_encrypted_str(P && enc) {
		m_encrypted = std::forward<P>(enc);
	}
	template<typename P> typename 
	std::enable_if<std::is_assignable<string_type, P>::value>::type set_decrypted_str(P && dec) {
		m_decrypted = std::forward<P>(dec);
	}

	/**
	 * @brief Polygon face count
	 * @param f face count
	 */
	void set_faces(const key_type& f) { m_faces = f; }
	const key_type& faces() { return m_faces; }

protected:
	string_type m_decrypted;
	string_type m_encrypted;
	key_type m_faces = 6;
};

class scytale_brute: public scytale {
public:
	using scytale::size_type;
	using scytale::string_type;
	using scytale::key_type;

public:
	using scytale::scytale;

	template<typename P> typename 
	std::enable_if<std::is_assignable<string_type, P>::value>::type set_search_str(P && key) {
		m_search_str = std::forward<P>(key);
	}
	const string_type& search_str() { return m_search_str; }

	std::list<key_type> brute(const key_type& from = 2, const key_type& to = 50, const size_type& max = 5);

	CA_CALLBACK(
		iteration,
		const key_type& key,
		const string_type& enc,
		const string_type& dec
	)

	CA_CALLBACK(
		found,
		const key_type& key,
		const string_type& enc,
		const string_type& dec
	)

protected:
	string_type m_search_str;
};

}

#include <calgo/nomacros.hpp>

#endif // !_CALGO_CRYPTOLOGY_SCYTALE_HPP_
