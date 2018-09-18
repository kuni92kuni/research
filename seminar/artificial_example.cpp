#include <iostream>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <kv/autodif.hpp>
#include <kv/affine.hpp>

class test {
public:
  kv::interval<double> f(kv::interval<double> x) {
    kv::interval<double> tmp;
    tmp = x * (x + 1) * (1 / x - 1 / (x + 1));
    return tmp * tmp - 2 * tmp;
  }

  void interval_result() {
    kv::interval<double> x, a;
    double epsilon = 1;
    double x_hat = 10000;
    for(int i = 0; i <= 13; i++) {
      x.lower() = -epsilon;
      x.upper() = epsilon;
      x = x + x_hat;
      a = f(x);
      std::cout << epsilon << " " << rad(a) << std::endl;
      epsilon /= 10;
    }
    return;
  }

  kv::autodif< kv::interval<double> > g(const kv::autodif< kv::interval<double> >& x) {
    return x * (x + 1) * (1 / x - 1 / (x + 1));
  }

  kv::autodif< kv::interval<double> > f(const kv::autodif< kv::interval<double> >& x) {
    return x * x - 2 * x;
  }

  double f(double x) {
    double tmp;
    tmp = x * (x + 1) * (1 / x - 1 / (x + 1));
    return tmp * tmp - 2 * tmp;
  }

  kv::interval<double> g(kv::interval<double> x) {
    return x * (x + 1) * (1 / x - 1 / (x + 1));
  }

  void average_result() {
    kv::interval<double> intx, diffx, a;
    kv::autodif< kv::interval<double> > autox;
    double epsilon = 1;
    double x_hat = 10000;

    for(int i = 0; i <= 13; i++) {
      intx.lower() = -epsilon;
      intx.upper() = epsilon;
      intx = intx + x_hat;
      autox = kv::autodif< kv::interval<double> >::init(intx);
      autox = f(g(autox));
      kv::autodif< kv::interval<double> >::split(autox2, a, diffx);
      //std::cout << epsilon << " " << diffx * (intx - x_hat) << std::endl;
      intx = f(x_hat) + diffx * (intx - x_hat);
      std::cout << epsilon << " " << rad(intx) << std::endl;
      epsilon /= 10;
    }
  }

  void average_result2() {
    kv::interval<double> intx, diffx1, diffx2, a;
    kv::autodif< kv::interval<double> > autoxg, autoxf;
    double epsilon = 1;
    double x_hat = 10000;

    for(int i = 0; i <= 13; i++) {
      intx.lower() = -epsilon;
      intx.upper() = epsilon;
      intx = intx + x_hat;
      autoxf = kv::autodif< kv::interval<double> >::init(g(intx));
      autoxg = kv::autodif< kv::interval<double> >::init(intx);
      autoxf = f(autoxf);
      autoxg = g(autoxg);
      kv::autodif< kv::interval<double> >::split(autoxg, a, diffx1);
      kv::autodif< kv::interval<double> >::split(autoxf, a, diffx2);
      //std::cout << epsilon << " " << diffx * (intx - x_hat) << std::endl;
      intx = f(x_hat) + diffx1 * diffx2 * (intx - x_hat);
      std::cout << epsilon << " " << rad(intx) << std::endl;
      epsilon /= 10;
    }
  }


  kv::affine<double> f(kv::affine<double> x) {
    return x * x - 2 * x;
  }

  kv::affine<double> g(kv::affine<double> x) {
    return x * (x + 1) * (1 / x - 1 / (x + 1));
  }

  void affine_result() {
    kv::affine<double> affx, a;
    kv::interval<double> intx;
    double epsilon = 1;
    double x_hat = 10000;
    
    for(int i = 0; i <= 13; i++) {
      intx.lower() = -epsilon;
      intx.upper() = epsilon;
      intx = intx + x_hat;
      affx = intx;
      a = to_interval(f(g(affx)));
      std::cout << epsilon << " " << rad(a) << std::endl;
      epsilon /= 10;
    }
  }

};

int main() {
  std::cout.setf(std::cout.scientific);
  std::cout.precision(16);

  test test;
//  test.interval_result();
  test.average_result();
//  test.average_result2();
//  test.affine_result();
}
