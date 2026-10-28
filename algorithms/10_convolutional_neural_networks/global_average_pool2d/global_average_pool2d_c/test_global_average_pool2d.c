#include "global_average_pool2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double a[]={1,2,3,4,5,6,7,8};
    const double neg[]={-3,-1};
    const double bad[]={1,NAN}, high[]={1,1001};
    double out[2]={99,99};
    assert(global_average_pool2d(a,2,2,2,out));
    assert(out[0]==2.5 && out[1]==6.5);
    assert(global_average_pool2d(neg,1,1,2,out) && out[0]==-2);
    assert(global_average_pool2d(neg,1,1,1,out) && out[0]==-3);
    out[0]=99;
    assert(!global_average_pool2d(a,0,2,2,out));
    assert(!global_average_pool2d(a,17,2,2,out));
    assert(!global_average_pool2d(a,2,0,2,out));
    assert(!global_average_pool2d(a,2,2,33,out));
    assert(!global_average_pool2d(bad,1,1,2,out));
    assert(!global_average_pool2d(high,1,1,2,out));
    assert(!global_average_pool2d(NULL,1,1,2,out));
    assert(!global_average_pool2d(a,2,2,2,NULL));
    assert(out[0]==99);
    puts("C global average pooling tests passed");
    return 0;
}
