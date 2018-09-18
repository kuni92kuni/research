#include <iostream>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <kv/affine.hpp>

class test {
public:
  double A(double x1, double x0) {
    return 3 * x1 - 2 * x0;
  }

  void double_result() {
    double x0 = 0.9;
    double x1 = 0.9;
    double x2;
    int i = 0;
    
    for(i = 2; i <= 30; i++) {
      x2 = A(x1, x0);
      x0 = x1;
      x1 = x2;
    }
    
    std::cout << "double" << std::endl;
    std::cout << i - 1 << " " << x2 << std::endl;
    
    return;
  }

  kv::interval<double> A(kv::interval<double> x1, kv::interval<double> x0) {
    return 3 * x1 - 2 * x0;
  }

  void interval_result() {
    kv::interval<double> x0("0.9");
    kv::interval<double> x1("0.9");
    kv::interval<double> x2;
    int i = 0;
    std::cout << x1 << std::endl;
    std::cout << rad(x1) << std::endl;
    
    for(i = 2; i <= 30; i++) {
      x2 = A(x1, x0);
      x0 = x1;
      x1 = x2;
    }

    
    //std::cout <<  << std::endl;
   
    std::cout << "interval" << std::endl;
    std::cout << i - 1 << " " << x2 << std::endl;
    
    return;
  }

  kv::affine<double> A(kv::affine<double> x1, kv::affine<double> x0) {
    return 3 * x1 - 2 * x0;
  }

  void affine_result() {
    kv::affine<double> x0(0.9);
    kv::affine<double> x1(0.9);
    kv::affine<double> x2;
    int i = 0;
   
    for(i = 2; i <= 30; i++) {
      x2 = A(x1, x0);
      x0 = x1;
      x1 = x2;
    }

//    std::cout << "affine1" << std::endl;
//    std::cout << "affine2" << std::endl;
    std::cout << "affine3" << std::endl;
    std::cout << i - 1 << " " << to_interval(x2) << std::endl;
    
    return; 
  }

};

  
int main() {
  std::cout.setf(std::cout.scientific);
  std::cout.precision(20);

  test test;
//  test.double_result();
  test.interval_result();
//  test.affine_result();
}
