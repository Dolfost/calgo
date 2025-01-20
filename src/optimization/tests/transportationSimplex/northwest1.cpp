#include <calgo/opt/test.hpp>

// from min1.cpp
ca::Vec<double> demand = {30, 20, 70, 30, 60};
ca::Vec<double> supply = {50, 60, 50};
ca::Mat<double> cost = {
	{16, 16, 13, 22, 17},
	{14, 14, 13, 19, 15},
	{19, 19, 20, 23, 10},
};

ca::Mat<double> res = {
	{30, 20, 0, 0, 0},
	{0, 0, 60, 0, 0},
	{0, 0, 10, 30, 10},
	{0, 0, 0, 0, 50},
};

int main(int argc, char** argv) {
	ca::Mat<double> dist = 
		ca::opt::TransportationSimplex<double>::addSlack(cost, demand, supply);
	ca::opt::TransportationSimplex<double>::cells_type cells(
		cost.rows()+cost.cols()-1
	);
	ca::opt::TransportationSimplex<double>::northWest(
		cost, demand, supply, cells, dist
	);

	std::cout << "expected:\n" << res << "got: \n" << dist;

	return res != dist;
}
