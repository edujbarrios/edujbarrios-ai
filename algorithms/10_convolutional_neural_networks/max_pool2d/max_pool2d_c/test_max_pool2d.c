#include "max_pool2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double full[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    const double odd[]={1,2,3,4,5,6,7,8,9}, neg[]={-4,-2,-1,-3};
    const double bad[]={1,NAN,3,4}, huge[]={1,1001,3,4};
    double out[4]={99,99,99,99};
    assert(max_pool2d(full,4,4,out));
    assert(out[0]==6 && out[1]==8 && out[2]==14 && out[3]==16);
    assert(max_pool2d(odd,3,3,out) && out[0]==5);
    assert(max_pool2d(neg,2,2,out) && out[0]==-1);
    out[0]=99;
    assert(!max_pool2d(full,0,4,out));
    assert(!max_pool2d(full,1,4,out));
    assert(!max_pool2d(full,4,1,out));
    assert(!max_pool2d(full,33,4,out));
    assert(!max_pool2d(bad,2,2,out));
    assert(!max_pool2d(huge,2,2,out));
    assert(!max_pool2d(NULL,4,4,out));
    assert(!max_pool2d(full,4,4,NULL));
    assert(out[0]==99);
    puts("C max-pooling tests passed");
    return 0;
}
