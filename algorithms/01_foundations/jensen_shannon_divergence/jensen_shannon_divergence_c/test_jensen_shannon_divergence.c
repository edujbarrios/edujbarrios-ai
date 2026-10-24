#include "jensen_shannon_divergence.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double a[]={1,0},b[]={0,1},c[]={.5,.5};
    const double one[]={1};
    const double negative[]={-.1,1.1};
    const double nan[]={NAN,1};
    const double incomplete[]={.2,.2};
    double result=-9;
    assert(jensen_shannon_divergence(a,b,2,&result) && fabs(result-1)<1e-12);
    assert(jensen_shannon_divergence(c,c,2,&result) && fabs(result)<1e-12);
    assert(jensen_shannon_divergence(a,c,2,&result));
    assert(fabs(result-.31127812445913283)<1e-12);
    assert(jensen_shannon_divergence(one,one,1,&result)&&result==0);
    result=-9;
    assert(!jensen_shannon_divergence(a,negative,2,&result));
    assert(!jensen_shannon_divergence(a,nan,2,&result));
    assert(!jensen_shannon_divergence(a,incomplete,2,&result));
    assert(!jensen_shannon_divergence(a,c,0,&result));
    assert(!jensen_shannon_divergence(a,c,2,NULL));
    assert(result==-9);
    puts("C JSD tests passed");
    return 0;
}
