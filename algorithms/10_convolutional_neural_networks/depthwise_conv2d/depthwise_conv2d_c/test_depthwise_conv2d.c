#include "depthwise_conv2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double input[]={1,2,3,4,5,6,7,8};
    const double kernels[]={0,0,0,0,1,0,0,0,0,
                            0,0,0,0,-1,0,0,0,0};
    const double ones[]={1,1,1,1,1,1,1,1,1};
    const double bad[]={NAN}, high[]={1001};
    double out[8]={99,99,99,99,99,99,99,99};
    assert(depthwise_conv2d(input,2,2,2,kernels,3,3,out));
    for(size_t i=0;i<4;++i) {
        assert(out[i]==input[i]);
        assert(out[i+4]==-input[i+4]);
    }
    assert(depthwise_conv2d(input,1,2,2,ones,3,3,out));
    for(size_t i=0;i<4;++i) assert(out[i]==10);
    assert(depthwise_conv2d(input,1,1,1,ones,3,3,out) && out[0]==1);
    out[0]=99;
    assert(!depthwise_conv2d(input,0,2,2,kernels,3,3,out));
    assert(!depthwise_conv2d(input,9,2,2,kernels,3,3,out));
    assert(!depthwise_conv2d(input,1,0,2,kernels,3,3,out));
    assert(!depthwise_conv2d(input,1,2,17,kernels,3,3,out));
    assert(!depthwise_conv2d(input,1,2,2,kernels,2,3,out));
    assert(!depthwise_conv2d(input,1,2,2,kernels,3,0,out));
    assert(!depthwise_conv2d(input,1,2,2,bad,1,1,out));
    assert(!depthwise_conv2d(input,1,2,2,high,1,1,out));
    assert(!depthwise_conv2d(NULL,1,2,2,kernels,3,3,out));
    assert(!depthwise_conv2d(input,1,2,2,kernels,3,3,NULL));
    assert(out[0]==99);
    puts("C depthwise conv2d tests passed");
    return 0;
}
