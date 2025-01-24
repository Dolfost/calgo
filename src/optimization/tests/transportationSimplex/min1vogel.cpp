#include <calgo/opt/test.hpp>

ca::Vec<double> demand = {30, 20, 70, 30, 60};
ca::Vec<double> supply = {50, 60, 50};
ca::Mat<double> cost = {
	{16, 16, 13, 22, 17},
	{14, 14, 13, 19, 15},
	{19, 19, 20, 23, 10},
};

CA_OPT_TRANSPORTATION_SIMPLEX_TEST(
	1970, [&simplex](){simplex.setBfs(ca::opt::TransportationSimplex<double>::BFS::Vogel); }
);
