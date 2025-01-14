#include <calgo/opt/test.hpp>

ca::Vec<double> func = {-2, -3, -4, 0, 0};
ca::Vec<double> constr = {10 ,15};
ca::Mat<double> vals = {
	{3, 2, 1, 1, 0},
	{2, 5, 3, 0 ,1},
};

CA_OPT_SIMPLEX_TEST(
	-20, [&]() { simplex.setMaximize(false); }
);
