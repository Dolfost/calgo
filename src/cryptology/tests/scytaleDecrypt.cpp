#include <calgo/cr/scytale.hpp>
#include <iostream>

int main(int argc, char** argv) {
	using namespace ca::cr;

	scytale s;
	s.set_faces(4);
	s.set_encrypted_str("Iryyatbhmvaehedlurlp");
	s.decrypt();

	std::string ex = "Iamhurtverybadlyhelp";
	std::cout << "expected: " << ex << "\ngot:      " << s.decrypted_str() << std::endl;

	return s.decrypted_str() != ex;
}
