#include "kullback_leibler_divergence.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double certain[]={1,0};
    const double opposite[]={0,1};
    const double half[]={.5,.5};
    const double uneven[]={.75,.25};
    const double incomplete[]={.2,.2};
    const double negative[]={1.1,-.1};
    const double nan[]={NAN,1};
    const double one[]={1};
    double result=-9;
    assert(kullback_leibler_divergence(half,half,2,&result)&&result==0);
    assert(kullback_leibler_divergence(certain,half,2,&result));
    assert(fabs(result-1)<1e-12);
    assert(kullback_leibler_divergence(uneven,half,2,&result));
    assert(fabs(result-.1887218755408672)<1e-12);
    assert(kullback_leibler_divergence(certain,opposite,2,&result));
    assert(isinf(result) && result>0);
    assert(kullback_leibler_divergence(one,one,1,&result)&&result==0);
    result=-9;
    assert(!kullback_leibler_divergence(half,incomplete,2,&result));
    assert(!kullback_leibler_divergence(half,negative,2,&result));
    assert(!kullback_leibler_divergence(half,nan,2,&result));
    assert(!kullback_leibler_divergence(half,half,0,&result));
    assert(!kullback_leibler_divergence(half,half,2,NULL));
    assert(result==-9);
    puts("C KL divergence tests passed");
    return 0;
}
