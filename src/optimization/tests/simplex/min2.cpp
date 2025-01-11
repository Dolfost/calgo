#include <calgo/opt/test.hpp>

ca::Vector<double> func = {-2, 3, 0, 0, 0};
ca::Vector<double> constr = {2, 10, 4};
ca::Matrix<double> vals = {
	{-1, 2, -1, 0, 0},
	{-5, 2, 0, 1, 0},
	{1, 2, 0, 0, -1},
};

CA_OPT_SIMPLEX_TEST(
	-8, [&]() { simplex.setMaximize(false); }
);
