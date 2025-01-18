#include <calgo/lin/test.hpp>

ca::Vec<double> solution = {
	-268.0/97,
	143.0/97,
	58.0/97,
	257.0/194
};

ca::Vec<double> constraints = {
	3, -4, -1, 7
};

ca::Mat<double> variables = {
	{5,	7, 2, 4},
	{7,	2, 3, 8},
	{6, 9, -5, 4},
	{7,	1, 15, 12}
};

CA_LIN_TEST_GAUSS(ca::lin::Gauss<double>)
