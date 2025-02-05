#include <calgo/opt/test.hpp>

ca::vec<double> func = {7, 6, 0, 0};
ca::vec<double> constr = {16, 12};
ca::mat<double> vals = {
	{2, 4, 1, 0},
	{3, 2, 0, 1},
};

CA_OPT_SIMPLEX_TEST(
	32
);
