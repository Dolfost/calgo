#ifndef _CALGO_CRYPTOLOGY_CIPHER_HPP_
#define _CALGO_CRYPTOLOGY_CIPHER_HPP_

#include <calgo/calgo.hpp>

#include <cstddef>

namespace ca::cr {

/**
 * @brief Base class for any cipher
 */
class cipher {
public:
	using size_type = std::size_t;

public:
	virtual const void* decrypted_data() const = 0;
	virtual const void* encrypted_data() const = 0;
	virtual void encrypt() = 0;
	virtual void decrypt() = 0;

	virtual size_type decrypted_data_size() const = 0;
	virtual size_type encrypted_data_size() const = 0;
};

}

#endif // !_CALGO_CRYPTOLOGY_CIPHER_HPP_
