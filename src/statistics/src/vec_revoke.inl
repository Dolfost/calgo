#pragma once
#include <calgo/ss/vec.hpp>

namespace ca::ss {

template<typename T> 
void vec_view<T>::revoke() {
	raw_moment.revoke();
	central_moment.revoke();
	std_dev.revoke();
	skew.revoke();
	kurtosis.revoke();
}

}
