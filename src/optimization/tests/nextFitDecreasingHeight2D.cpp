#include <calgo/opt/test.hpp>

CALGO_OPT_DEFAULT_TEST(
	ca::opt::NextFit2D, int,
	[](ca::opt::NextFit2D<int>& p) {
		p.setComparator(
			[](const ca::opt::Box2D<int>* a, const ca::opt::Box2D<int>* b) {
				return a->size().height() > b->size().height();
			}
		);
	}
)
