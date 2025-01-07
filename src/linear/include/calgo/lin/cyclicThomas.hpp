#ifndef _CALGO_LIN_CYCLIC_THOMAS_HPP_
#define _CALGO_LIN_CYCLIC_THOMAS_HPP_

#include <calgo/calgo.hpp>

namespace ca::lin {

class CyclicThomas {
public:
	static ListT solve(
		const ListT&,
		const ListT&,
		const ListT&,
		const ListT&
	);
};

}

#endif // !_CALGO_LIN_CYCLIC_THOMAS_HPP_
