#ifndef _CALGO_LSYS_CYCLIC_THOMAS_HPP_
#define _CALGO_LSYS_CYCLIC_THOMAS_HPP_

#include <calgo/calgo.hpp>

namespace ca::lsys {

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

#endif // !_CALGO_LSYS_CYCLIC_THOMAS_HPP_
