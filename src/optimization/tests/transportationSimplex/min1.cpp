#include <calgo/opt/test.hpp>

ca::vec<double> demand = {30, 20, 70, 30, 60};
ca::vec<double> supply = {50, 60, 50};
ca::mat<double> cost = {
	{16, 16, 13, 22, 17},
	{14, 14, 13, 19, 15},
	{19, 19, 20, 23, 10},
};

CA_OPT_TRANSPORTATION_SIMPLEX_TEST(
	1970
);
