#include <calgo/cr/scytale.hpp>
#include <iostream>

int main(int argc, char** argv) {
	using namespace ca::cr;

	scytale s;
	s.set_faces(5);
	s.set_decrypted_str("istextisnotmultipleofke");
	s.encrypt();

	std::string ex =  "ittifsimpktsuleenle xoto ";
	std::cout << "expected: |" << ex << "|\ngot:      |" << s.encrypted_str() << '|' << std::endl;

	return s.encrypted_str() != ex;
}
