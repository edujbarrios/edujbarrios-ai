#include "dense_forward.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {2.0,-1.0};
    const double w[] = {1.0,1.0,-1.0,2.0};
    const double b[] = {0.0,1.0};
    const double scalar_x[] = {1.0};
    const double scalar_w[] = {3.0};
    const double scalar_b[] = {2.0};
    const double nonfinite[] = {NAN};
    const double huge[] = {1e308};
    double out[] = {-9.0,-9.0};
    assert(dense_forward(x,2,w,b,2,out));
    assert(out[0] == 1.0 && out[1] == 0.0);
    assert(dense_forward(scalar_x,1,scalar_w,scalar_b,1,out));
    assert(out[0] == 5.0);
    out[0] = -9.0;
    assert(!dense_forward(x,0,w,b,2,out));
    assert(!dense_forward(x,2,w,b,0,out));
    assert(!dense_forward(x,2,nonfinite,b,1,out));
    assert(!dense_forward(huge,1,huge,scalar_b,1,out));
    assert(!dense_forward(x,2,w,b,2,NULL));
    assert(out[0] == -9.0);
    puts("C dense forward tests passed");
    return 0;
}
