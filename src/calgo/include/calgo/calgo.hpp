#ifndef _CALGO_CALGO_HPP_
#define _CALGO_CALGO_HPP_

#include <vector>

namespace ca {

using ListT = std::vector<double>;

static struct {
	const long double pi = 3.141592653589793238462643383279502884197;
	const long double e = 2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274;
	const long double phi = (1 + std::sqrt(5))/2;
	const long double invphi = 1/phi;
} mconst;

enum class Op {
	Less = -1,
	Equal = 0,
	Greater = 1,
};

}

#endif // !_CALGO_CALGO_HPP_
