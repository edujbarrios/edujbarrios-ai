#include "gaussian_kde_1d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void near(double a, double b) { assert(fabs(a-b) < 1e-12); }
int main(void) {
    const double one[] = {0.0}, twin[] = {-1.0, 1.0}, same[] = {1.0, 1.0};
    const double invalid[] = {1.0, NAN}, out_bound[] = {1001.0};
    const double s = 2.5066282746310005024;
    double result = -1.0;
    assert(gaussian_kde_1d(one,1,0.0,1.0,&result));
    near(result,1.0/s);
    assert(gaussian_kde_1d(twin,2,0.0,1.0,&result));
    near(result,exp(-0.5)/s);
    assert(gaussian_kde_1d(same,2,1.0,2.0,&result));
    near(result,1.0/(2.0*s));
    assert(gaussian_kde_1d(one,1,1000.0,0.001,&result));
    assert(result == 0.0);
    double many[512] = {0.0};
    assert(gaussian_kde_1d(many,512,0.0,1.0,&result));
    near(result,1.0/s);
    assert(gaussian_kde_1d(one,1,-1000.0,1000.0,&result));
    near(result,exp(-0.5)/(1000.0*s));
    result = 42.0;
    assert(!gaussian_kde_1d(NULL,1,0,1,&result));
    assert(!gaussian_kde_1d(one,0,0,1,&result));
    assert(!gaussian_kde_1d(one,513,0,1,&result));
    assert(!gaussian_kde_1d(one,1,NAN,1,&result));
    assert(!gaussian_kde_1d(one,1,1001,1,&result));
    assert(!gaussian_kde_1d(one,1,0,0,&result));
    assert(!gaussian_kde_1d(one,1,0,0.0001,&result));
    assert(!gaussian_kde_1d(one,1,0,1001,&result));
    assert(!gaussian_kde_1d(one,1,0,INFINITY,&result));
    assert(!gaussian_kde_1d(invalid,2,0,1,&result));
    assert(!gaussian_kde_1d(out_bound,1,0,1,&result));
    assert(!gaussian_kde_1d(one,1,0,1,NULL));
    assert(result == 42.0);
    puts("C Gaussian KDE tests passed");
    return 0;
}
