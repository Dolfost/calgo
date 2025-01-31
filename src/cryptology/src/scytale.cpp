#include <calgo/cr/scytale.hpp>

#include <calgo/cr/freq_analysis.hpp>

namespace ca::cr {

void scytale::encrypt() {
	m_encrypted.resize(m_decrypted.length());

	// matrix width
	const size_type w = (m_decrypted.length() + m_faces - 1) / m_faces;
	for (size_type i = 0; i < m_decrypted.length(); i++)
		m_encrypted[(i % w) * m_faces + (i / w)] = m_decrypted[i];
}

void scytale::decrypt() {
	m_decrypted.resize(m_encrypted.length());

	// matrix width
	const size_type w = (m_encrypted.length() + m_faces - 1) / m_faces;
	// opposite to encryption
	for (size_type i = 0; i < m_encrypted.length(); i++)
		m_decrypted[(i % m_faces) * w + (i / m_faces)] = m_encrypted[i];
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
