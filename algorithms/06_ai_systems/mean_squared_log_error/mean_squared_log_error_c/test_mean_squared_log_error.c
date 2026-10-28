#include "mean_squared_log_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double target[]={0,1,9},prediction[]={0,3,9};
    const double one[]={1},zero[]={0},big[]={1000000};
    const double neg[]={-1},nan[]={NAN},inf[]={INFINITY};
    const double too_big[]={1000001};
    double out=99;
    assert(mean_squared_log_error(target,prediction,3,&out));
    assert(fabs(out-log(2.0)*log(2.0)/3.0)<1e-12);
    assert(mean_squared_log_error(target,target,3,&out) && out==0.0);
    assert(mean_squared_log_error(zero,one,1,&out));
    assert(fabs(out-log(2.0)*log(2.0))<1e-12);
    assert(mean_squared_log_error(zero,big,1,&out));
    assert(fabs(out-pow(log1p(1000000.0),2))<1e-11);
    out=99;
    assert(!mean_squared_log_error(neg,one,1,&out));
    assert(!mean_squared_log_error(one,neg,1,&out));
    assert(!mean_squared_log_error(nan,one,1,&out));
    assert(!mean_squared_log_error(inf,one,1,&out));
    assert(!mean_squared_log_error(too_big,one,1,&out));
    assert(!mean_squared_log_error(one,one,0,&out));
    assert(!mean_squared_log_error(one,one,10001,&out));
    assert(!mean_squared_log_error(NULL,one,1,&out));
    assert(!mean_squared_log_error(one,one,1,NULL));
    assert(out==99);
    puts("C MSLE tests passed");
    return 0;
}
