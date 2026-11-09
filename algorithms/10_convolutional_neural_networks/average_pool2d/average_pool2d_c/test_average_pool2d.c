#include "average_pool2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
static void check(const double *image,size_t h,size_t w,const double *want) {
    double actual[256]={0};
    assert(average_pool2d(image,h,w,actual));
    for(size_t i=0;i<(h/2)*(w/2);++i)assert(fabs(actual[i]-want[i])<1e-12);
}
int main(void) {
    const double image[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    const double expected[]={3.5,5.5,11.5,13.5};
    check(image,4,4,expected);
    const double odd[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    const double odd_expected[]={4,6};
    check(odd,3,5,odd_expected);
    const double four[]={1,2,3,4},single[]={2.5};
    check(four,2,2,single);
    const double negatives[]={-4,-2,-6,-8},negative_expected[]={-5};
    check(negatives,2,2,negative_expected);
    const double zeros[]={0,0,0,0},zero[]={0};
    check(zeros,2,2,zero);
    const double high[]={1000,1000,1000,1000},high_expected[]={1000};
    check(high,2,2,high_expected);
    const double horizontal[]={1,2,3,4,5,6},row_expected[]={3};
    check(horizontal,2,3,row_expected);
    check(horizontal,3,2,single);
    double large[1024],out[256];
    for(size_t i=0;i<1024;++i)large[i]=1;
    assert(average_pool2d(large,32,32,out));
    for(size_t i=0;i<256;++i)assert(out[i]==1);
    for(size_t r=0;r<32;++r)
        for(size_t c=0;c<32;++c)large[r*32+c]=(r+c)%2?-1.0:1.0;
    assert(average_pool2d(large,32,32,out));
    for(size_t i=0;i<256;++i)assert(out[i]==0);
    out[0]=777;
    const double bad[]={1,2,3,NAN},inf[]={1,2,3,INFINITY};
    const double huge[]={1,2,3,1001},low[]={1,2,3,-1001};
    assert(!average_pool2d(NULL,2,2,out));
    assert(!average_pool2d(four,2,2,NULL));
    assert(!average_pool2d(four,1,2,out));
    assert(!average_pool2d(four,2,1,out));
    assert(!average_pool2d(four,33,2,out));
    assert(!average_pool2d(four,2,33,out));
    assert(!average_pool2d(bad,2,2,out));
    assert(!average_pool2d(inf,2,2,out));
    assert(!average_pool2d(huge,2,2,out));
    assert(!average_pool2d(low,2,2,out));
    double alias[]={1,2,3,4};
    assert(!average_pool2d(alias,2,2,alias));
    assert(out[0]==777);
    puts("C average pooling tests passed");
    return 0;
}
