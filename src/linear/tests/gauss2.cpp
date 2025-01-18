#include <calgo/lin/test.hpp>

ca::Vec<double> solution = {
	74783/17650.0,
	-132857/52950.0,
	-44767/26475.0,
	2693/2118.0,
	-18873/17650.0,
};

ca::Vec<double> constraints = {
	5, 6, 15, 2, 4
};

ca::Mat<double> variables = {
	{ 15, 8, 9,-4,17},
	{ 3, 2, 5, 7, 2},
	{ 3,-7, 5,-2, 4},
	{ 5, 7, 8, 6,-4},
	{ 1, 7,-7, 6, 2},
};

CA_LIN_TEST_GAUSS(ca::lin::Gauss<double>)
