#include "winsorized_mean.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *v, size_t n, double f, double expected) {
    double result = -777;
    assert(winsorized_mean(v,n,f,&result));
    assert(fabs(result-expected)<=1e-12*fmax(1.,fabs(expected)));
}
int main(void) {
    const double v1[]={1,2,3,4,100}, v2[]={1,2,3,100};
    const double v3[]={100,3,2,1}, v4[]={1,5,100};
    const double single[]={42}, large[]={-1e308,1e308};
    const double no_trim[]={0,2,100}, sample[]={1,2,3,100,1000};
    check(v1,5,0.2,3);
    check(v2,4,0.25,2.5);
    check(v2,4,0,26.5);
    check(v3,4,0.25,2.5);
    check(v4,3,0.49,5);
    check(single,1,0.49,42);
    check(large,2,0,0);
    check(no_trim,3,0.3,34);
    check(sample,5,0.2,41.4);
    double long_v[512];
    for(size_t i=0;i<512;++i)long_v[i]=7;
    check(long_v,512,0.49,7);
    double result=777;
    const double invalid[]={1,NAN};
    assert(!winsorized_mean(NULL,1,0,&result));
    assert(!winsorized_mean(v1,0,0,&result));
    assert(!winsorized_mean(v1,513,0,&result));
    assert(!winsorized_mean(v1,5,-0.1,&result));
    assert(!winsorized_mean(v1,5,0.5,&result));
    assert(!winsorized_mean(v1,5,INFINITY,&result));
    assert(!winsorized_mean(v1,5,NAN,&result));
    assert(!winsorized_mean(invalid,2,0,&result));
    assert(!winsorized_mean(v1,5,0,NULL));
    assert(result==777);
    puts("C winsorized mean tests passed");
    return 0;
}
