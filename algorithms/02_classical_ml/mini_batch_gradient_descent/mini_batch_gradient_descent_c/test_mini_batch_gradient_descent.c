#include "mini_batch_gradient_descent.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[]={0,1}, y[]={1,3};
    const double train_x[]={0,1,2,3}, train_y[]={1,3,5,7};
    const double bad_x[]={NAN,1};
    const double huge[]={1e308,1e308};
    double w=-9,b=-9;
    assert(mini_batch_gradient_descent(x,y,2,0.1,1,1,&w,&b));
    assert(fabs(w-0.29)<1e-12 && fabs(b-0.39)<1e-12);
    assert(mini_batch_gradient_descent(train_x,train_y,4,0.05,2000,3,&w,&b));
    assert(fabs(w-2.0)<0.01 && fabs(b-1.0)<0.01);
    w=-9;b=-9;
    assert(!mini_batch_gradient_descent(x,y,0,0.1,1,1,&w,&b));
    assert(!mini_batch_gradient_descent(x,y,2,0.0,1,1,&w,&b));
    assert(!mini_batch_gradient_descent(x,y,2,0.1,0,1,&w,&b));
    assert(!mini_batch_gradient_descent(x,y,2,0.1,1,0,&w,&b));
    assert(!mini_batch_gradient_descent(x,y,2,0.1,1,3,&w,&b));
    assert(!mini_batch_gradient_descent(bad_x,y,2,0.1,1,1,&w,&b));
    assert(!mini_batch_gradient_descent(huge,y,2,1e308,2,2,&w,&b));
    assert(!mini_batch_gradient_descent(x,y,2,0.1,1,1,&w,&w));
    assert(w==-9 && b==-9);
    puts("C mini-batch gradient descent tests passed");
    return 0;
}
