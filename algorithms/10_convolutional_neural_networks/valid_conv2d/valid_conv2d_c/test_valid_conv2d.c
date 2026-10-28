#include "valid_conv2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double image[]={1,2,3,4,5,6,7,8,9};
    const double kernel[]={1,0,0,-1}, scale[]={-2};
    const double bad[]={1,NAN,0,-1}, huge[]={1,1001,0,-1};
    double out[9]={99,99,99,99,99,99,99,99,99};
    assert(valid_conv2d(image,3,3,kernel,2,2,out));
    for (size_t i=0;i<4;++i) assert(out[i]==-4);
    assert(valid_conv2d(image,3,3,scale,1,1,out));
    for (size_t i=0;i<9;++i) assert(out[i]==-2*image[i]);
    const double rectangular[]={1,2,3,4,5,6};
    assert(valid_conv2d(rectangular,2,3,kernel,2,2,out));
    assert(out[0]==-4 && out[1]==-4);
    out[0]=99;
    assert(!valid_conv2d(image,0,3,kernel,2,2,out));
    assert(!valid_conv2d(image,3,0,kernel,2,2,out));
    assert(!valid_conv2d(image,33,3,kernel,2,2,out));
    assert(!valid_conv2d(image,3,3,kernel,0,2,out));
    assert(!valid_conv2d(image,3,3,kernel,4,2,out));
    assert(!valid_conv2d(image,3,3,bad,2,2,out));
    assert(!valid_conv2d(image,3,3,huge,2,2,out));
    assert(!valid_conv2d(NULL,3,3,kernel,2,2,out));
    assert(!valid_conv2d(image,3,3,kernel,2,2,NULL));
    assert(out[0]==99);
    puts("C valid conv2d tests passed");
    return 0;
}
