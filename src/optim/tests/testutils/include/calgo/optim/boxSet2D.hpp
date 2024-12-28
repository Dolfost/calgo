#ifndef _CALGO_OPTIM_TEST_BOXSET_HPP_
#define _CALGO_OPTIM_TEST_BOXSET_HPP_

#include <calgo/optim/packing2D.hpp>

#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

static const std::vector<std::vector<ca::optim::Box2D<int>>> boxSet = {
	{
		{5, 8}, {7, 3}, {7, 15}, {9, 4}, {8,10}, {2, 5}, {8, 12},
	}, {
		{6, 3}, {15, 4}, {10, 6}, {8, 3}, {11, 7}, {5, 12}, {7, 10},
	},
};

template<typename T>
std::vector<ca::optim::Box2D<T>> binaryBs(T p) {
	std::vector<ca::optim::Box2D<T>> bs;
	for (int i = 0; i < std::log2(p); ++i) {
		T side = std::pow(2, std::log2(p) - i);
		int num_boxes = std::pow(2, i);
		for (int j = 0; j < num_boxes; ++j)
			bs.push_back(ca::optim::Box2D<T>(side, side));
	}	
	std::shuffle(bs.begin(), bs.end(), std::default_random_engine(std::random_device()()));
	return bs;
}

#endif // !_CALGO_OPTIM_TEST_BOXSET_HPP_
