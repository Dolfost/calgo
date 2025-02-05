#include <calgo/opt/test.hpp>

ca::vec<double> func = {1, -5, -4, 1, 16, 0, 0, 0};
ca::vec<double> constr = {13, 5, -2};
ca::mat<double> vals = {
	{10, 5, 8, -2, -4, 1, 0, 0},
	{11, -1, 6, 1, 10, 0, 1, 0},
	{-14, 6, -2, -7, 9, 0, 0, -1},
};

CA_OPT_SIMPLEX_TEST(
	8
);
