#include <calgo/optim/test.hpp>

CALGO_OPTIM_DEFAULT_TEST(
	ca::optim::FirstFit2D, int,
	[](ca::optim::FirstFit2D<int>& p) {
		p.setComparator(
			[](const ca::optim::Box2D<int>* a, const ca::optim::Box2D<int>* b) {
				return a->area() < b->area();
			}
		);
	}
)
