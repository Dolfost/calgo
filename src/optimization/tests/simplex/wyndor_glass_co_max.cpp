#include <calgo/opt/test.hpp>

ca::Vec<double> func = {3, 2, 0, 0, 0};
ca::Vec<double> constr = {4, 12, 18};
ca::Mat<double> vals = {
	{1, 0, 1, 0, 0},
	{0, 2, 0, 1, 0},
	{3, 2, 0, 0, 1},
};

CA_OPT_SIMPLEX_TEST(
	18
);
