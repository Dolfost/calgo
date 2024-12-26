#include <calgo/interpol/lagrange.hpp>

namespace ca::interpol {

void Lagrange
::setX(ListT x) {
	ExplicitInterpolation::setX(x);
	calculateL();
}

void Lagrange::calculateL() {
	double res = 1.0;

	for(int i = 0; i < i_n; i++){

		for(int j = 0; j < i_n; j++){
			if(j == i) continue;
			else {
				res *= (i_x[i] - i_x[j]);
			}
		}

		n_L.push_back(res);
		res = 1.0;
	}

}

double Lagrange::interpolate(double x){
	double sum = 0.0;
	double res = 1.0;

	for(int i = 0; i < i_n; i++){
		for(int j = 0; j < i_n; j++){
			if(j == i) continue;
			else {
				res *= (x - i_x[j]);
			}
		}

		res *= i_y[i];
		res /= n_L[i];
		sum += res;
		res = 1.0;
	}

	return sum;
}

}
