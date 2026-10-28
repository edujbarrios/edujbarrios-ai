#include "weighted_linear_regression_1d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={0,1,2},y[]={1,2,2},weight[]={1,1,2};
    const double straight[]={1,3,5},uniform[]={1,1,1};
    const double zeros[]={0,0,0},single_weight[]={1,0,0};
    const double constant[]={2,2,2},bad[]={1,NAN,1};
    const double negative[]={1,-1,1},large[]={1,1000001,1};
    const double two_x[]={0,1},two_y[]={1,3},two_w[]={1,1};
    double slope=99,intercept=98;
    assert(weighted_linear_regression_1d(x,y,weight,3,&slope,&intercept));
    assert(fabs(slope-5.0/11.0)<1e-12);
    assert(fabs(intercept-13.0/11.0)<1e-12);
    assert(weighted_linear_regression_1d(x,straight,uniform,3,&slope,&intercept));
    assert(fabs(slope-2.0)<1e-12 && fabs(intercept-1.0)<1e-12);
    assert(weighted_linear_regression_1d(two_x,two_y,two_w,2,&slope,&intercept));
    assert(fabs(slope-2.0)<1e-12 && fabs(intercept-1.0)<1e-12);
    slope=99;intercept=98;
    assert(!weighted_linear_regression_1d(x,y,zeros,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,single_weight,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(constant,y,uniform,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,bad,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,negative,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,large,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,uniform,1,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,uniform,129,&slope,&intercept));
    assert(!weighted_linear_regression_1d(NULL,y,uniform,3,&slope,&intercept));
    assert(!weighted_linear_regression_1d(x,y,uniform,3,&slope,&slope));
    assert(!weighted_linear_regression_1d(x,y,uniform,3,NULL,&intercept));
    assert(slope==99 && intercept==98);
    puts("C weighted linear regression tests passed");
    return 0;
}
