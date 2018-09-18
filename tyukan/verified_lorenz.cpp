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

void init_matrix(ub::matrix<double>& m, int size_x, int size_y, int size_z, int w) {
	int number;
	for(int i = 0; i < size_z; i++) {
		for(int j = 0; j < size_y; j++) {
			for(int k = 0; k < size_x; k++) {
				number = size_x * size_y * i + size_x * j + k;
				m(0,number) = w * k - 30;
				m(1,number) = w * j - 40;
				m(2,number) = w * i;
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

void approximate_solution(ub::matrix<double>& m, int total, int end) {
  ub::vector<double> v;

  for(int i = 0; i < end; i++) {
		for(int j = 0; j < total; j++) {
			v = column(m, j);
			kv::rk(Func(), v, 0.001 * j, 0.001 * (j + 1));
			for(int k = 0; k < 3; k++) {
				m(k, j) = v(k);
			}
		}
  }

}

void select_refpoint(ub::matrix<double> m, ub::vector<double> min_dist,
	ub::vector<int>& is_exist, int total1, int& error, int size_x, int size_y, int size_z) {
	int x, y, z, number;
	ub::vector<double> v;

	for(int i = 0; i < total1; i++) {
		if(min_dist(i) < 1) {
			v = column(m, i);
			if (v(0) < -30 || v(0) > 30 || v(1) < -40 || v(1) > 40 || v(2) < 0 || v(2) > 60) {
				error = 1;
				std::cout << "error" << std::endl;
				break;
			}
			if(v(0) >= 0) {
				x = (int)v(0);
			} else {
				x = (int)v(0) - 1;
			}
			if(v(1) >= 0) {
				y = (int)v(1);
			} else {
				y = (int)v(1) - 1;
			}
			if(v(2) >= 0) {
				z = (int)v(2);
			} else {
				z = (int)v(2) - 1;
			}
			x += 30;
			y += 40;

			number = size_x * size_y * z + size_x * y + x;

			is_exist(number) = 1;
			is_exist(number + 1) = 1;
			is_exist(number + size_x) = 1;
			is_exist(number + size_x * size_y) = 1;
			is_exist(number + 1 + size_x) = 1;
			is_exist(number + size_x + size_x * size_y) = 1;
			is_exist(number + size_x * size_y + 1) = 1;
			is_exist(number + size_x + size_x * size_y + 1) = 1;
		}
	}
}

void print_matrix2(ub::matrix<double> m, int total, ub::vector<double> min_dist) {
	for(int i = 0; i < total; i++) {
		if(min_dist(i) < 1) {
			for(int j = 0; j < 3; j++) {
				std::cout << m(j, i) << " ";
			}
			std::cout << std::endl;
		}
	}
}

int main() {
	int size_x = 13;
	int size_y = 17;
	int size_z = 13;
	int total1 = size_x * size_y * size_z;
	ub::matrix<double> m(3, total1);
	int error = 1;

	ub::vector<double> min_dist(total1);
	ub::vector<double> a(3), b(3);
	double dist;

  /*近似解の生成 初期値の幅 w = 5*/
	init_matrix(m, size_x, size_y, size_z, 5);

  approximate_solution(m, total1, 1000);

	/*離れている点の削除*/
	for(int i = 0; i < total1; i++) {
		a = column(m, i);
		for(int j = 0; j < total1; j++) {
			if(i != j) {
				b = column(m, j);
				dist = d(a, b);
				if(j == 0 || (i == 0 && j == 1)) {
					min_dist(i) = dist;
				} else {
					if(min_dist(i) > dist) {
						min_dist(i) = dist;
					}
				}
			}
		}
	}

	//print_vector(min_dist, total1);
	print_matrix2(m, total1, min_dist);

  //print_matrix(m, total1);
  /*初期点の生成 幅 w = 1*/
  size_x = 61;
  size_y = 81;
  size_z = 61;
  int total2 = size_x * size_y * size_z;
  ub::matrix<double> refpoint(3, total2);
	ub::vector<int> is_exist(total2);

	//init_matrix(refpoint, size_x, size_y, size_z, 1);

	//select_refpoint(m, min_dist, is_exist, total1, error, size_x, size_y, size_z);

	//std::cout << m(0, 0) << " " << m(1, 0) << " " << m(2, 0) << std::endl;
	if(error != 1) {
		for(int i = 0; i < total2; i++) {
			if(is_exist(i) == 1) {
				std::cout << refpoint(0, i) << " " << refpoint(1, i) << " " << refpoint(2, i) << std::endl;
			}
		}
	}


  /*double a = 0.1;
  int b;
  b = a;
  if(b == 0) {
    std::cout << "success" << std::endl;
  }*/



	//print_matrix(m, total);

	//ub::vector<double> v(3), a(3), b(3), min(total);

	//norm_max(m, total, 100, 10);


	//print_vector(min, total);



	//print_matrix(m, total);

	

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
