#include <calgo/opt/test.hpp>

ca::Vector<double> func = {7, 6, 0, 0};
ca::Vector<double> constr = {16, 12};
ca::Matrix<double> vals = {
	{2, 4, 1, 0},
	{3, 2, 0, 1},
};

CA_OPT_SIMPLEX_TEST(
	32
);
