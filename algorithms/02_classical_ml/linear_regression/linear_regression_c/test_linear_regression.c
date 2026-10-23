#include "linear_regression.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {0,1,2,3};
    const double y[] = {1,3,5,7};
    const double horizontal[] = {3,3,3,3};
    const double constant[] = {2,2,2,2};
    const double bad[] = {NAN,1};
    const double huge[] = {1e308,-1e308};
    double slope=-99.0, intercept=-99.0;

    assert(linear_regression(x,y,4,&slope,&intercept));
    assert(fabs(slope-2.0)<1e-12 && fabs(intercept-1.0)<1e-12);
    assert(linear_regression(x,horizontal,4,&slope,&intercept));
    assert(slope==0.0 && intercept==3.0);
    slope=-99.0; intercept=-99.0;
    assert(!linear_regression(constant,y,4,&slope,&intercept));
    assert(!linear_regression(x,y,1,&slope,&intercept));
    assert(!linear_regression(bad,y,2,&slope,&intercept));
    assert(!linear_regression(huge,huge,2,&slope,&intercept));
    assert(!linear_regression(x,y,4,&slope,&slope));
    assert(slope==-99.0 && intercept==-99.0);
    puts("C linear regression tests passed");
    return 0;
}
