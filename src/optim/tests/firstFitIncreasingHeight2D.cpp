#include <calgo/optim/test.hpp>

CALGO_OPTIM_DEFAULT_TEST(
	ca::optim::SortedFirstFit2D, int,
	[](ca::optim::SortedFirstFit2D<int>& p) {
		p.setComparator(
			[](const ca::optim::Box2D<int>& a, const ca::optim::Box2D<int>& b) {
				return a.size().height() < b.size().height();
			}
		);
	}
)