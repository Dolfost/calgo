#include <calgo/cr/random.hpp>

#include <cmath>
#include <algorithm>

namespace ca::cr::FIBS_140_1 {

monobit_result monobit_test(const std::uint8_t* data) {
	monobit_result res;
	for (std::size_t i = 0; i < sample_size*8; i++)
		if (data[i/8] & (1 << i%8))
			res.ones++;
	if (not (res.ones < res.ones_from or res.ones > res.ones_to))
		res.passed = true;

	return res;
}

poker_result poker_test(const std::uint8_t* data) {
	poker_result res;
	std::size_t freqs[16] = {0};
	for (std::size_t i = 0; i < sample_size*2; i++)
		freqs[data[i/2] | ( 0xF << (i%2)*4)]++;

	for (std::size_t i = 0; i < 16; i++)
		res.statistic += std::pow(freqs[i], 2);
	res.statistic *= 16/5000.0;
	if (res.statistic > res.range.a and res.statistic < res.range.b)
		res.passed = true;
	return res;
}

series_length_result series_length_test(const std::uint8_t* data) {
	series_length_result res;
	bool current = data[0] & 1;
	std::size_t tmp = 0;
	for (std::size_t i = 0; i < sample_size*8; i++)
		if ((data[i/8] & (1 << i%8)) == current) {
			tmp++;
		} else {
			res.length = std::max(tmp, res.length);
			current = !current;
			tmp = 0;
		}

	res.length = std::max(tmp, res.length);
	if (res.length <= res.max_length)
		res.passed = true;

	return res;
}

series_result series_test(const std::uint8_t* data) {
	series_result res;
	bool current = data[0] & 1;
	std::size_t current_len = 0;
	for (std::size_t i = 0; i < sample_size*8; i++) {
		if ((data[i/8] & (1 << i%8)) == current)
			current_len++;
		else if (current_len >= 2) {
			res.quantities[current_len-2]++;
			current = not current;
		}

		if (current_len >= 6) {
			i++;
			if (i < sample_size*8 and (data[i/8] & (1 << i%8)) == current) {
				while (i < sample_size*8 and (data[i/8] & (1 << i%8)) == current)
					i++;
				current = not current;
			} else {
				res.quantities[current_len - 2]++;
			}
			current_len = 0;
			continue;
		}
	}

	res.passed = true;
	for (std::size_t i = 0; i < 5; i++)
		if (not res.ranges[i].contains(res.quantities[i])) {
			res.passed = false;
			break;
		}

	return res;
}

}
