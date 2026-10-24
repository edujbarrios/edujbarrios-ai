#include "ridge_regression_1d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[]={0,1,2},y[]={1,3,5};
    const double same[]={2,2,2},other[]={1,2,3};
    const double missing[]={0,NAN,2},high[]={0,1000001,2};
    double intercept=-9,slope=-9;
    assert(ridge_regression_1d(x,y,3,0,&intercept,&slope));
    assert(fabs(intercept-1.0)<1e-12 && fabs(slope-2.0)<1e-12);
    assert(ridge_regression_1d(x,y,3,2,&intercept,&slope));
    assert(fabs(intercept-2.0)<1e-12 && fabs(slope-1.0)<1e-12);
    assert(fabs((intercept+3*slope)-5.0)<1e-12);
    assert(ridge_regression_1d(same,other,3,0,&intercept,&slope));
    assert(fabs(intercept-2.0)<1e-12 && slope==0.0);
    assert(ridge_regression_1d(x,y,1,0,&intercept,&slope));
    assert(intercept==1.0 && slope==0.0);
    intercept=-9;slope=-9;
    assert(!ridge_regression_1d(x,y,0,0,&intercept,&slope));
    assert(!ridge_regression_1d(x,y,3,-1,&intercept,&slope));
    assert(!ridge_regression_1d(x,y,3,INFINITY,&intercept,&slope));
    assert(!ridge_regression_1d(missing,y,3,0,&intercept,&slope));
    assert(!ridge_regression_1d(high,y,3,0,&intercept,&slope));
    assert(!ridge_regression_1d(x,y,3,0,NULL,&slope));
    assert(intercept==-9 && slope==-9);
    puts("C ridge regression 1D tests passed");
    return 0;
}
