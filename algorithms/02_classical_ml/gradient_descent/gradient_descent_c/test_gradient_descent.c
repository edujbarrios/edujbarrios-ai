#include "gradient_descent.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {0, 1, 2, 3};
    const double y[] = {1, 3, 5, 7};
    const double horizontal[] = {3, 3, 3, 3};
    const double bad[] = {NAN};
    double weight=-1.0, bias=-1.0;

    assert(gradient_descent(x,y,4,0.05,2000,&weight,&bias));
    assert(fabs(weight-2.0)<1e-3 && fabs(bias-1.0)<1e-3);
    assert(gradient_descent(x,horizontal,4,0.05,2000,&weight,&bias));
    assert(fabs(weight)<1e-3 && fabs(bias-3.0)<1e-3);
    weight=-1.0; bias=-1.0;
    assert(!gradient_descent(x,y,0,0.05,10,&weight,&bias));
    assert(!gradient_descent(x,y,4,0.0,10,&weight,&bias));
    assert(!gradient_descent(x,y,4,-0.1,10,&weight,&bias));
    assert(!gradient_descent(x,y,4,0.05,0,&weight,&bias));
    assert(!gradient_descent(bad,y,1,0.05,10,&weight,&bias));
    assert(!gradient_descent(x,y,4,0.05,10,&weight,&weight));
    assert(weight==-1.0 && bias==-1.0);
    puts("C gradient descent tests passed");
    return 0;
}
