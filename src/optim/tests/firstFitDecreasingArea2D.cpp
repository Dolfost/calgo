#include <calgo/optim/test.hpp>

CALGO_OPTIM_TESTING_SORTED(
	ca::optim::SortedFirstFit2D, 
	[](const ca::optim::Box2D<int>& a, const ca::optim::Box2D<int>& b) {
		return a.area() > b.area();
	}
)
