#include <calgo/opt/test.hpp>

CALGO_OPT_DEFAULT_TEST(
	ca::opt::FirstFit2D, int,
	[](ca::opt::FirstFit2D<int>& p) {
		p.setComparator(
			[](const ca::opt::Box2D<int>* a, const ca::opt::Box2D<int>* b) {
				return a->area() > b->area();
			}
		);
	}
)
