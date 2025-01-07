#ifndef _CALGO_NUM_SIMPSON_HPP_
#define _CALGO_NUM_SIMPSON_HPP_

#include <calgo/calgo.hpp>

namespace ca::num {

class Simpson {
public:
	void setY(const ListT& l) {
		s_y = l;
	}
	void setX(const ListT& l) {
		s_x = l;
	}
	double integrate();

	double h(std::size_t i) {
		return s_x[i+1] - s_x[i];
	}

protected:
	ListT s_y, s_x;
};

}

#endif // !_CALGO_NUM_SIMPSON_HPP_
