#include <calgo/opt/test.hpp>

ca::Vec<double> demand = {50, 10, 65, 25, 10};
ca::Vec<double> supply = {40, 60, 70, 25};
ca::Mat<double> cost = {
	{8, 4, 1, 2, 1},
	{7, 1, 7, 3, 8},
	{3, 7, 8, 7, 5},
	{1, 3, 6, 7, 2}
};

CA_OPT_TRANSPORTATION_SIMPLEX_TEST(
	1230, [&simplex]() { simplex.setMaximize(true); }
);
