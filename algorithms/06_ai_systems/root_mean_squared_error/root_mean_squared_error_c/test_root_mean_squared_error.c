#include "root_mean_squared_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double target[]={0,1,2},prediction[]={0,2,4};
    const double negative[]={-1000000},positive[]={1000000};
    const double zero[]={0},one[]={1};
    const double bad[]={NAN},inf[]={INFINITY},large[]={1000001};
    double out=99;
    assert(root_mean_squared_error(target,prediction,3,&out));
    assert(fabs(out-sqrt(5.0/3.0))<1e-12);
    assert(root_mean_squared_error(target,target,3,&out) && out==0);
    assert(root_mean_squared_error(negative,positive,1,&out) &&
           fabs(out-2000000.0)<1e-9);
    assert(root_mean_squared_error(zero,one,1,&out) && out==1);
    out=99;
    assert(!root_mean_squared_error(zero,bad,1,&out));
    assert(!root_mean_squared_error(inf,zero,1,&out));
    assert(!root_mean_squared_error(zero,large,1,&out));
    assert(!root_mean_squared_error(zero,one,0,&out));
    assert(!root_mean_squared_error(zero,one,10001,&out));
    assert(!root_mean_squared_error(NULL,one,1,&out));
    assert(!root_mean_squared_error(zero,one,1,NULL));
    assert(out==99);
    puts("C RMSE tests passed");
    return 0;
}
