#include <calgo/cr/scytale.hpp>
#include <iostream>

int main(int argc, char** argv) {
	using namespace ca::cr;

	scytale_brute s;
	s.set_encrypted_str("Iryyatbhmvaehedlurlp");
	s.set_search_str("help");
	s.iterationCallback([](auto k, auto enc, auto dec) {
		std::cout << "key: " << k << "\nenc:\n" << enc << "\ndec:\n" << dec << std::endl << std::endl;
	});

	return s.brute(2, 10) != std::list<scytale_brute::key_type>{4};
}
