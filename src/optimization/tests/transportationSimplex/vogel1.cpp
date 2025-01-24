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
	{0, 0, 50, 0, 0},
	{10, 20, 20, 0, 10},
	{0, 0, 0, 0, 50},
	{20, 0, 0, 30, 0}
};
ca::opt::TransportationSimplex<double>::cells_type bas = {
	{3,3}, {3,0}, {2,4}, {0,2}, {1,2}, {1,0}, {1,1}, {1,4}
};

int main(int argc, char** argv) {
	ca::Mat<double> dist = 
		ca::opt::TransportationSimplex<double>::addSlack(cost, demand, supply);
	ca::opt::TransportationSimplex<double>::cells_type cells(
		cost.rows()+cost.cols()-1
	);
	ca::opt::TransportationSimplex<double>::vogel(
		cost, demand, supply, cells, dist
	);

	std::cout << "expected:\n" << res << "bas: ";
	for (auto& x: bas) {
		std::cout << '{' << x.i << ',' << x.j << "} ";
	}
	std::cout << "\n\ngot:\n" << dist << "bas: ";
	for (auto& x: cells) {
		std::cout << '{' << x.i << ',' << x.j << "} ";
	}
	std::cout << std::endl;

	return (res != dist) or (bas != cells);
}
