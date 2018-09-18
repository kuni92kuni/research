#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <kv/ode-maffine.hpp>
#include <kv/ode-qr-lohner.hpp>
#include <kv/rk.hpp>

#include <math.h>

namespace ub = boost::numeric::ublas;
typedef kv::interval<double> itv;


struct Func {
	template <class T> ub::vector<T> operator() (const ub::vector<T>& x, T t) {
		ub::vector<T> y(3);

		double p = 10;
		double r = 28;
		double b = 8/3;

		y(0) = -p * x(0) + p * x(1);
		y(1) = -x(0) * x(2) + r * x(0) - x(1);
		y(2) = x(0) * x(1) -b * x(2);

		return y;
	}
};

void init_matrix(ub::matrix<double>& m, int size_x, int size_y, int size_z, int width) {
	int number;
	for(int i = 0; i < size_z; i++) {
		for(int j = 0; j < size_y; j++) {
			for(int k = 0; k < size_x; k++) {
				number = size_x * size_y * i + size_x * j + k;
				m(0,number) = width * k - 30;
				m(1,number) = width * j - 40;
				m(2,number) = width * i;
			}
		}
	}
}

void print_matrix(ub::matrix<double> m, int total) {
	for(int i = 0; i < total; i++) {
		for(int j = 0; j < 3; j++) {
			std::cout << m(j, i) << " ";
		}
		std::cout << std::endl;
	}
}

void print_vector(ub::vector<double> v, int total) {
	for(int i = 0; i < total; i++) {
		std::cout << v(i) << std::endl;
	}
}

double d(ub::vector<double> a, ub::vector<double> b) {
	return sqrt(pow(a(0) - b(0), 2) + pow(a(1) - b(1), 2) + pow(a(2) - b(2), 2)) ;
}

double max(ub::vector<double> v, int total) {
	double max;
	for(int i = 0; i < total; i++) {
		if(i == 0) {
			max = v(i);
		}
		if(max < v(i)) {
			max = v(i);
		}
	}
	return max;
}

void norm_max(ub::matrix<double> m, int total, int end, int width) {
	ub::matrix<double> n(3, total);
	ub::vector<double> v(3), a(3), b(3), min(total);
	double min_dist, dist;

	for(int i = 0; i <= end; i++) {
		for(int j = 0; j < total; j++) {
			v = column(m, j);
			kv::rk(Func(), v, 0.001 * j, 0.001 * (j + 1));
			for(int k = 0; k < 3; k++) {
				m(k, j) = v(k);
			}
		}
		if(i % width == 0) {
			if(i != 0){
				for(int g = 0; g < total; g++) {
					for(int h = 0; h < total; h++) {
						a = column(m, g);
						b = column(n, h);
						dist = d(a, b);
						if(h == 0) {
							min_dist = dist;
						}
						if(min_dist > dist) {
							min_dist = dist;
						}
					}
					min(g) = min_dist;
				}
				std::cout << i << " " << max(min, total) << std::endl;
			}
			n = m;
		}
	}

}


int main() {
	int size_x = 31;
	int size_y = 41;
	int size_z = 31;
	int total = size_x * size_y * size_z;
	ub::matrix<double> m(3, total);

	init_matrix(m, size_x, size_y, size_z, 2);

	//print_matrix(m, total);

	ub::vector<double> v(3), a(3), b(3), min(total);

	norm_max(m, total, 100000, 1000);


	//print_vector(min, total);



	//print_matrix(m, total);

	/*ub::vector<double> v(3);
	v(0) = 1; v(1) = 2; v(2) = 3;
	for(int i = 0; i < 10000; i++) {
		kv::rk(Func(), v, i * 0.001, (i + 1) * 0.001);
		std::cout << v(0) << " " << v(1) << " " << v(2) << std::endl;
	}
	*/

	//kv::rk()
	/*
	if(r == 0) {
		std::cout << "Cannot calculate solution\n";
	} else if (r == 1) {
		std::cout << "Solution calculated until t = " << end << ".\n";
		std::cout << x << "\n";
	} else {
		std::cout << "Solution calculated.\n";
		std::cout << x << "\n";
	}
	*/
}
