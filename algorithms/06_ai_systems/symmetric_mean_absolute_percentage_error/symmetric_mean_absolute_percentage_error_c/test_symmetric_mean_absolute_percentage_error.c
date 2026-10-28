#include "symmetric_mean_absolute_percentage_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double t[]={0,1,-1},p[]={0,2,1},same[]={0,1,-1};
    const double positive[]={1},negative[]={-1},zero[]={0};
    const double high[]={1000000},low[]={-1000000};
    const double invalid[]={NAN},infinite[]={INFINITY},large[]={1000001};
    double out=99.0;
    assert(symmetric_mean_absolute_percentage_error(t,p,3,&out));
    assert(fabs(out-8.0/9.0)<1e-12);
    assert(symmetric_mean_absolute_percentage_error(t,same,3,&out) && out==0.0);
    assert(symmetric_mean_absolute_percentage_error(zero,zero,1,&out) && out==0.0);
    assert(symmetric_mean_absolute_percentage_error(positive,zero,1,&out) && out==2.0);
    assert(symmetric_mean_absolute_percentage_error(positive,negative,1,&out) && out==2.0);
    assert(symmetric_mean_absolute_percentage_error(high,low,1,&out) && out==2.0);
    out=99.0;
    assert(!symmetric_mean_absolute_percentage_error(invalid,zero,1,&out));
    assert(!symmetric_mean_absolute_percentage_error(infinite,zero,1,&out));
    assert(!symmetric_mean_absolute_percentage_error(large,zero,1,&out));
    assert(!symmetric_mean_absolute_percentage_error(t,p,0,&out));
    assert(!symmetric_mean_absolute_percentage_error(t,p,10001,&out));
    assert(!symmetric_mean_absolute_percentage_error(NULL,p,1,&out));
    assert(!symmetric_mean_absolute_percentage_error(t,p,1,NULL));
    assert(out==99.0);
    puts("C sMAPE tests passed");
    return 0;
}
