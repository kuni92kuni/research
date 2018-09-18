#include <iostream>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <kv/affine.hpp>

class test {
public:
  kv::interval<double> fx(kv::interval<double> x, kv::interval<double> y) {
    return 1 - 1.05 * x * x + y;
  }

  kv::interval<double> fy(kv::interval<double> x, kv::interval<double> y) {
    return 0.3 * x;
  }

  void interval_result() {
//    kv::interval<double> x0(-1e-5, 1e-5);
//    kv::interval<double> y0(-1e-5, 1e-5);
    kv::interval<double> x0(0);
    kv::interval<double> y0(0);
    kv::interval<double> x1, y1;
    int i = 0;

    std::cout << i << " " << rad(x0) << std::endl;
    
    for(i = 1; i < 200; i++) {
      x1 = fx(x0, y0);
      y1 = fy(x0, y0);
      if(rad(x1) > rad(y1)) {
        std::cout << i << " " << rad(x1) << std::endl;
      } else {
        std::cout << i << " " << rad(y1) << std::endl;
      }
      x0 = x1;
      y0 = y1;
    }
    return;
  }

  kv::affine<double> fx(kv::affine<double> x, kv::affine<double> y) {
    return 1 - 1.05 * x * x + y;
  }

  kv::affine<double> fy(kv::affine<double> x, kv::affine<double> y) {
    return 0.3 * x;
  }

  void affine_result() {
//    kv::interval<double> intx(-1e-5, 1e-5);
//    kv::interval<double> inty(-1e-5, 1e-5);
    kv::interval<double> intx(0);
    kv::interval<double> inty(0);
    kv::affine<double> affx0, affx1, affy0, affy1;
    affx0 = intx;
    affy0 = inty;
    int i = 0;

    std::cout << i << " " << rad(affx0) << std::endl;

    for(i = 1; i <= 200; i++) {
      affx1 = fx(affx0, affy0);
      affy1 = fy(affx0, affy0);
      if(rad(affx1) > rad(affy1)) {
        std::cout << i << " " << rad(affx1) << std::endl;
      } else {
        std::cout << i << " " << rad(affy1) << std::endl;
      }
      affx0 = affx1;
      affy0 = affy1;
    }
    return;
  }

};

  
int main() {
  std::cout.setf(std::cout.scientific);
  std::cout.precision(16);

  test test;
//  test.interval_result();
  test.affine_result();
}

