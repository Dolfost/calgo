#ifndef _CALGO_OPTIM_TESTING_TEST_HPP_
#define _CALGO_OPTIM_TESTING_TEST_HPP_

#include <calgo/optim/packing2D.hpp>
#include <calgo/optim/draw.hpp>
#include <calgo/optim/boxSet2D.hpp>

#include <string>
#include <iostream>

static int calgo_optim_testing_test_no = 1;

template<template<class> class P, typename N=int>
bool test(P<N>& packing, std::vector<ca::optim::Box2D<int>> boxes, std::string msg) {
	std::vector<ca::optim::Box2D<int>> rboxes = packing.pack(boxes);

	std::cout 
		<< calgo_optim_testing_test_no 
		<< " ===> " <<  msg << "\nInput boxes:\t\t" 
	  <<  "Output boxes (in bin " << packing.size() << ", area: " << packing.area() << "):\n";
	for (int i = 0; i < boxes.size(); i++)
		std::cout
			<< boxes[i] << "\t\t"
			<< rboxes[i] << '\n';
	drawBoxesRow<int>(boxes);
	std::cout << "- - - - - - - - - -\n";
	drawBoxesPlane<int>(rboxes, packing);
	return false;
}

template<template<class> class P, typename N=int>
bool default_test(std::vector<ca::optim::Box2D<int>> boxes, std::string msg) {
	P<N> ff;
	return test<P, N>(ff, boxes, msg);
}

template<template<class> class P, typename N=int>
bool default_test_sorted(
	std::vector<ca::optim::Box2D<int>> boxes, 
	typename ca::optim::SortedPacking2D<N>::Comp comp,
	std::string msg
) {
	P<N> ff;
	ff.setComparator(comp);
	return test<P, N>(ff, boxes, msg);
}


#define CALGO_OPTIM_TESTING_DEFAULT(CLS) \
int main(int argc, char** argv) { \
	for (std::size_t i = 0; i < boxSet.size(); i++) \
		default_test<CLS>(boxSet[i], "box set №" + std::to_string(i)); \
	default_test<CLS>(binaryBs<int>(16), "binary set 16"); \
	return 0; \
}

#define CALGO_OPTIM_TESTING_SORTED(CLS, COMP) \
int main(int argc, char** argv) { \
	for (std::size_t i = 0; i < boxSet.size(); i++) \
		default_test_sorted<CLS>(boxSet[i], COMP, "box set №" + std::to_string(i)); \
	default_test_sorted<CLS>(binaryBs<int>(16), COMP, "binary set 16"); \
	return 0; \
}

#endif // !_CALGO_OPTIM_TESTING_TEST_HPP_
