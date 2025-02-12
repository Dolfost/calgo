#include <calgo/cr/random.hpp>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <random>
#include <cstring>

int main(int argc, char** argv) {
	using T = unsigned int;
	ca::cr::bbs<T> l;
	l.set_seed(654873);

	std::random_device d;
	std::uint8_t data[ca::cr::FIPS_140_2::sample_size];
	for (std::size_t i = 0; i < ca::cr::FIPS_140_2::sample_size; i += sizeof(T)) {
		T rand = d() xor l.generate() xor clock();
		std::cout << std::uppercase << std::hex << std::bitset<8*sizeof(T)>(rand) << std::dec << '\n';
		std::memcpy(data + i, &rand, sizeof(T));
	}

	ca::cr::FIPS_140_2::monobit_result mr = 
		ca::cr::FIPS_140_2::monobit_test(data);
	std::cout << (mr.passed ? "PASS" : "FAIL" ) << " - monobit test: " <<
		mr.range.a << " <= " << mr.ones << " <= " << mr.range.b << std::endl;

	ca::cr::FIPS_140_2::poker_result pr = 
		ca::cr::FIPS_140_2::poker_test(data);
	std::cout << (pr.passed ? "PASS" : "FAIL" ) << " - poker test: " <<
		pr.range.a << " <= " << pr.statistic << " <= " << pr.range.b << std::endl;
	
	ca::cr::FIPS_140_2::long_run_result slr = 
		ca::cr::FIPS_140_2::long_run_test(data);
	std::cout << (slr.passed ? "PASS" : "FAIL" ) << " - series length test: " <<
		slr.length << " <= " << slr.max_length << std::endl;

	ca::cr::FIPS_140_2::runs_result sr = 
		ca::cr::FIPS_140_2::runs_test(data);
	std::cout << (sr.passed ? "PASS" : "FAIL" ) << " - series test: \n";
	for (std::size_t i = 0; i < std::size(ca::cr::FIPS_140_2::runs_result::ranges); i++) {
		std::cout << "    " << (sr.ranges[i].contains(sr.quantities[i]) ? "PASS" : "FAIL" ) 
			<< " - count of series of " << i + 1 << " " << sr.ranges[i].a << " <= " << sr.quantities[i] << " <= " << sr.ranges[i].b << "\n";
	}

	return 0;
}
