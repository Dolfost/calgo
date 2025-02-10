#include <calgo/cr/random.hpp>
#include <iostream>
#include <bitset>

int main(int argc, char** argv) {
	ca::cr::bbs<unsigned long> l;
	for (int i = 0; i < 100; i += 1) {
		std::cout << std::bitset<8*sizeof(unsigned long)>(l.generate()) << std::endl;
	}
	

	return 0;
}
