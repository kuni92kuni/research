#include <stdio.h>
#include <math.h>

int initial(double *x, double *y, double *start, double *end, double *h) {
  int step;
  scanf("%d", &step);
  *x = 1;
  *y = 0;
  *start = 0;
  *end = 6 * M_PI;
  *h = 6 * M_PI / step;
  return step;
}

double fx(double x, double y) {
  return -y;
}

double fy(double x, double y) {
  return x;
}

//void euler(double x0, double y0, double start, double end, double h, int step) {
double euler(double x0, double y0, double start, double end, double h, int step) {
  double t = start;
  double x1, y1;
  int i;
  //printf("%f %f\n",x0, y0);
  for(i = 0; i < step; i++) {
    x1 = x0 + h * fx(x0, y0);
    y1 = y0 + h * fy(x0, y0); 
    //printf("%f %f\n",x1, y1);
    x0 = x1;
    y0 = y1;
  }
  return sqrt(x1 * x1 + y1 * y1);
}


int main() {
  double x, y, start, end, h, r;
  int step;
  step = initial(&x, &y, &start, &end, &h);
  //euler(x, y, start, end, h, step);
  r = euler(x, y, start, end, h, step);
  printf("h = pi / %4d r = %f\n", step, r);
  return 0;
}
