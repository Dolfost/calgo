#include <calgo/opt/floyd_warshall.hpp>

int main(int argc, char** argv) {
	ca::opt::floyd_warshall<int> fw;
	const int& none = fw.no_edge;
	fw.solve_safe(
		ca::mat<int>({ { 0, 5, none, 10 },
			{ none, 0, 3, none },
			{ none, none, 0, 1 },
			{ none, none, none, 0 } })
	);

	std::cout << fw.distances();

	return fw.distances() != ca::mat<int>{ {0, 5, 8, 9},
		{none, 0,   3,   4},
		{none, none, 0,   1},
		{none, none, none, 0},
	};
}

