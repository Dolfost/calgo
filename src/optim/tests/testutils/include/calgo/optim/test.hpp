#ifndef _CALGO_OPTIM_TESTING_TEST_HPP_
#define _CALGO_OPTIM_TESTING_TEST_HPP_

#include <calgo/optim/packing2D.hpp>
#include <calgo/optim/draw.hpp>
#include <calgo/optim/boxSet2D.hpp>

#include <string>
#include <iostream>
#include <functional>

static int calgo_optim_testing_test_no = 1;

template<template<class> class P, typename N=int>
bool test(std::vector<ca::optim::Box2D<N>*> boxes, std::string msg, std::function<void(P<N>&)> lambda = [](P<N>&){}) {
	P<N> packing;
	lambda(packing);
	
	N boxArea = 0;
	for (auto& box: boxes) {
		boxArea += box->area();
	}
	if (calgo_optim_testing_test_no != 1)
		std::cout << '\n';

	std::cout << "test № "
		<< calgo_optim_testing_test_no 
		<< " ===> " <<  msg 
		<< "\nInput boxes ("
		<< boxes.size() << "pcs, " << boxArea << "u²):\n";
	for (int i = 0; i < boxes.size(); i++)
		std::cout << boxes[i]->size() << " ";

	std::cout << std::endl;
	drawBoxesRow<N>(boxes);

	packing.pack(boxes);

	N area = packing.area();
	double occupied = 100*double(boxArea)/area;
	double whilespace = 100-occupied;
	std::cout <<  "\nOutput boxes (" << packing.size() << "=" 
		<< area << "u², occupied: " << occupied 
		<< "%, whitespace: " << whilespace << "%):\n";

	if (occupied > 100)
		throw std::logic_error("more than 100% is occupied, impossible.");

	for (int i = 0; i < boxes.size(); i++)
		std::cout
			<< *boxes[i] << " ";

	drawBoxesPlane<N>(boxes, packing);
	calgo_optim_testing_test_no++;
	return false;
}

#define CALGO_OPTIM_DEFAULT_TEST(CLS, N, LAMBDA) \
int main(int argc, char** argv) { \
	for (std::size_t i = 0; i < boxSet.size(); i++) { \
			std::vector<ca::optim::Box2D<N>*> v; \
			for (const auto& x: boxSet[i]) { \
				v.push_back(new ca::optim::Box2D<N>(x)); \
			} \
			test<CLS, N>(v, "box set №" + std::to_string(i+1), LAMBDA); \
			for (auto& x: v) { \
				delete x; \
			} \
	} \
	std::vector<ca::optim::Box2D<N>*> v = binaryBs<int>(16); \
	test<CLS, N>(v, "binary set 16", LAMBDA); \
	for (auto& x: v) { \
		delete x; \
	} \
	return 0; \
}


#endif // !_CALGO_OPTIM_TESTING_TEST_HPP_
