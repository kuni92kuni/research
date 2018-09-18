#include <iostream>
#include <kv/ode-maffine.hpp>
#include <kv/ode-qr-lohner.hpp>

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

int main()
{
	ub::vector<itv> x;
	itv end;
	int r;

	std::cout.precision(17);

	x.resize(3);
	x(0) = 1.;
	x(1) = 1.;
	x(2) = 1.;

	end = 10;
	r = kv::odelong_qr_lohner(Func(), x, itv(0.), end, kv::ode_param<double>().set_order(5));

	if(r == 0) {
		std::cout << "Cannot calculate solution\n";
	} else if (r == 1) {
		std::cout << "Solution calculated until t = " << end << ".\n";
		std::cout << x << "\n";
	} else {
		std::cout << "Solution calculated.\n";
		std::cout << x << "\n";
	}
}
