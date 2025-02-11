#include <calgo/cr/random.hpp>
#include <iostream>
#include <cstdint>
#include <bitset>

int main(int argc, char** argv) {
	using T = unsigned int;
	ca::cr::bbs<T> l;
	l.set_seed(654873);

	std::uint8_t data[ca::cr::FIBS_140_1::sample_size];
	for (std::size_t i = 0; i < ca::cr::FIBS_140_1::sample_size/sizeof(T); i += 1) {
		T rand = l.generate();
		std::cout << std::uppercase << std::hex << std::bitset<8*sizeof(T)>(rand) << std::dec << '\n';
		*((T*)(data) + i) = rand;
	}
	ca::cr::FIBS_140_1::monobit_result mr = 
		ca::cr::FIBS_140_1::monobit_test(data);
	std::cout << (mr.passed ? "PASS" : "FAIL" ) << " - monobit test: " <<
		mr.ones_from << " <= " << mr.ones << " <= " << mr.ones_to << std::endl;

	ca::cr::FIBS_140_1::poker_result pr = 
		ca::cr::FIBS_140_1::poker_test(data);
	std::cout << (pr.passed ? "PASS" : "FAIL" ) << " - poker test: " <<
		pr.range.a << " <= " << pr.statistic << " <= " << pr.range.b << std::endl;
	
	ca::cr::FIBS_140_1::series_length_result slr = 
		ca::cr::FIBS_140_1::series_length_test(data);
	std::cout << (slr.passed ? "PASS" : "FAIL" ) << " - series length test: " <<
		slr.length << " <= " << slr.max_length << std::endl;

	ca::cr::FIBS_140_1::series_result sr = 
		ca::cr::FIBS_140_1::series_test(data);
	std::cout << (sr.passed ? "PASS" : "FAIL" ) << " - series test: \n";
	for (std::size_t i = 0; i < std::size(ca::cr::FIBS_140_1::series_result::ranges); i++) {
		std::cout << "    " << (sr.ranges[i].contains(sr.quantities[i]) ? "PASS" : "FAIL" ) 
			<< " - count of series of " << i + 2 << " " << sr.ranges[i].a << " <= " << sr.quantities[i] << " <= " << sr.ranges[i].b << "\n";
	}

	return 0;
}
