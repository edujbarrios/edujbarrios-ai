#include "quantile_linear_interpolation.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
static void near(const double *v,size_t n,double p,double want) {
    double out=-777;
    assert(quantile_linear_interpolation(v,n,p,&out));
    assert(fabs(out-want)<=1e-11*fmax(fabs(want),1.0));
}
int main(void) {
    const double a[]={1,2,3,4},b[]={5,10},single[]={42};
    const double unsorted[]={100,2,-5}, extremes[]={-1e308,1e308};
    near(a,4,0.25,1.75);
    near(a,4,0.50,2.5);
    near(a,4,0.75,3.25);
    near(b,2,0.2,6);
    near(single,1,0.9,42);
    near(unsorted,3,0,-5);
    near(unsorted,3,1,100);
    near(extremes,2,0.5,0);
    near(extremes,2,0,-1e308);
    near(extremes,2,1,1e308);
    double many[512];
    for(size_t i=0;i<512;++i)many[i]=7;
    near(many,512,0.37,7);
    double out=777;
    const double bad[]={1,NAN},infinite[]={INFINITY};
    assert(!quantile_linear_interpolation(NULL,1,0.5,&out));
    assert(!quantile_linear_interpolation(a,0,0.5,&out));
    assert(!quantile_linear_interpolation(a,513,0.5,&out));
    assert(!quantile_linear_interpolation(a,4,-0.01,&out));
    assert(!quantile_linear_interpolation(a,4,1.01,&out));
    assert(!quantile_linear_interpolation(a,4,NAN,&out));
    assert(!quantile_linear_interpolation(a,4,INFINITY,&out));
    assert(!quantile_linear_interpolation(a,4,0.5,NULL));
    assert(!quantile_linear_interpolation(bad,2,0.5,&out));
    assert(!quantile_linear_interpolation(infinite,1,0.5,&out));
    assert(out==777);
    puts("C quantile tests passed");
    return 0;
}
