#pragma once
#include <calgo/num/intervalMinimum.hpp>

#include <stdexcept>

namespace ca::num {

template<typename T>
void IntervalMinimum<T>::check() const {
	if (not m_func)
		throw std::runtime_error("ca::num::InervalMinimum: no function");
	if (m_int.is_inverted())
		throw std::runtime_error("ca::num::InervalMinimum: inverted interval");
}

}
