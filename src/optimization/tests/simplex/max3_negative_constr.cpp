#include <calgo/opt/test.hpp>

// degenerate and unbounded
ca::Vec<double> func = {5, -5, 4, 13, 7, 0, 0, 0};
ca::Vec<double> constr = {-7, 2, -5};
ca::Mat<double> vals = {
	{-1, 12, 6, -1, -6, 0, 0, 0},
	{0, 3, 1, 14, -12, 0, 0, 0},
	{2, 4, -5, -3, -1, 0, 0, -1},
};

CA_OPT_SIMPLEX_TEST(
	34.2727272
);
