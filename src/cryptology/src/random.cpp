#include <calgo/cr/random.hpp>

#include <cmath>
#include <algorithm>

namespace ca::cr::FIPS_140_2 {

monobit_result monobit_test(const std::uint8_t* data) {
	monobit_result res;
	for (std::size_t i = 0; i < sample_size*8; i++)
		if (data[i/8] & (1 << i%8))
			res.ones++;
	if (res.range.contains(res.ones))
		res.passed = true;

	return res;
}

poker_result poker_test(const std::uint8_t* data) {
	poker_result res;
	std::size_t freqs[16] = {0};
	for (std::size_t i = 0; i < sample_size*2; i++)
		freqs[(data[i/2] >> (4*(i%2))) & 0x0F]++;

	for (std::size_t i = 0; i < 16; i++)
		res.statistic += (std::pow(freqs[i], 2));
	res.statistic *= (16/5000.0);
	res.statistic -= 5000;
	if (res.range.contains(res.statistic))
		res.passed = true;
	return res;
}

long_run_result long_run_test(const std::uint8_t* data) {
	long_run_result res;
	bool current = data[0] & 1;
	std::size_t tmp = 0;
	for (std::size_t i = 0; i < sample_size*8; i++)
		if ((data[i/8] & (1 << i%8)) xor current) {
			res.length = std::max(tmp, res.length);
			current = !current;
			tmp = 0;
		} else
			tmp++;

	res.length = std::max(tmp, res.length);
	if (res.length <= res.max_length)
		res.passed = true;

	return res;
}

// 	BUG: counts are wrong
runs_result runs_test(const std::uint8_t* data) {
	runs_result res;
	bool current = data[0] & 1;
	std::size_t current_len = 1;
	for (std::size_t i = 1; i < sample_size*8; i++)
		if (current_len < 6) {
			if (bool(data[i/8] & (1 << (i%8))) != current) { // if different
				res.quantities[current_len-1]++;
				current = not current;
				current_len = 1;
			} else // if same
				current_len++;
		} else { // current_len == 6
			while (i < sample_size*8 and bool(data[i/8] & (1 << (i%8))) == current)
				i++;
			res.quantities[6-1]++;
			current = not current;
			current_len = 1;
		}

	res.passed = true;
	for (std::size_t i = 0; i < sizeof(res.quantities)/sizeof(std::size_t); i++)
		if (not res.ranges[i].contains(res.quantities[i])) {
			res.passed = false;
			break;
		}

	return res;
}

}
