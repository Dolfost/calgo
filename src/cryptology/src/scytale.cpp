#include <calgo/cr/scytale.hpp>

#include <calgo/cr/freq_analysis.hpp>
#include <cassert>

namespace ca::cr {

void scytale::encrypt() {
	const size_type padding = (m_faces - m_decrypted.length() % m_faces) % m_faces;
	m_encrypted.resize(m_decrypted.length() + padding);

	// width
	const size_type b = (m_decrypted.length() + padding) / m_faces;
	for (string_type::size_type i = 0; i < m_decrypted.length() + padding; i++) {
		const size_type idx = (i % m_faces) * b + i / m_faces;

		assert(idx < m_encrypted.length());

		if (idx >= m_decrypted.length())
			m_encrypted[i] = ' ';
		else
			m_encrypted[i] = m_decrypted[idx];
	}
}

void scytale::decrypt() {
	const size_type padding = (m_faces - m_encrypted.length() % m_faces) % m_faces;
	m_decrypted.resize(m_encrypted.length() + padding);

	// width
	const size_type b = (m_encrypted.length() + padding) / m_faces;
	for (string_type::size_type i = 0; i < m_encrypted.length() + padding; i++) {
		const size_type idx = (i / b) + (i % b) * m_faces;

		assert(idx < m_encrypted.length() + padding);

		if (idx >= m_encrypted.length())
			m_decrypted[i] = ' ';
		else
			m_decrypted[i] = m_encrypted[idx];
	}
}

std::list<scytale_brute::key_type> scytale_brute::brute(const key_type& from, const key_type& to, const size_type& max) {
	std::list<key_type> found_keys;
	const bool call = (bool)p_iterationCallback;
	const bool call_found = (bool)p_foundCallback;
	for (key_type i = from; i < to; i++) {
		set_faces(i);
		decrypt();
		if (decrypted_str().find(m_search_str) != string_type::npos) {
			found_keys.push_back(i);
			if (call_found)
				p_foundCallback(i, encrypted_str(), decrypted_str());
			if (found_keys.size() >= max)
				break;
		}
		if (call)
			p_iterationCallback(i, encrypted_str(), decrypted_str());
	}

	return std::move(found_keys);
}

}
