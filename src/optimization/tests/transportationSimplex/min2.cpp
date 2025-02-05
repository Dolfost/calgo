#include <calgo/opt/test.hpp>

ca::vec<double> demand = {5,	8,	7,	14};
ca::vec<double> supply = {7, 9, 18};
ca::mat<double> cost = {
	{19,	30,	50,	10},
	{70,	30,	40,	60},
	{40,	8,	70,	20}
};

CA_OPT_TRANSPORTATION_SIMPLEX_TEST(
	743
);
