#include <calgo/interpol/newtonBackward.hpp>

namespace ca::interpol {

void NewtonBackward::setX(ListT x) {
	ExplicitInterpolation::setX(x);
	calculateDiff();
}

void NewtonBackward::calculateDiff() {
	n_dif.clear();

	std::vector<ListT> diff(i_n);
	diff[0] = i_y;
	n_dif.push_back(diff[0].back());

	for (int i = 1; i < i_n; i++) {
		diff[i].resize(i_n-i);
		for (int j = 0; j < i_n-i; j++)
			diff[i][j] = (diff[i - 1][j] - diff[i - 1][j + 1]) /
				(i_x[j] - i_x[i + j]);
		n_dif.push_back(diff[i].back());
	}
}

double NewtonBackward::interpolate(double x) {
	double val = 0;
	for (int i = 0; i < i_n; i++) {
		double prod = n_dif[i];
		for (int j = i_n-1; j >= i_n-i ; j--)
			prod *= (x - i_x[j]);
		val += prod;
	}

	return val;
}

}
