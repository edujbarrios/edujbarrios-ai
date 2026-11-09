#include "same_conv2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void check(const double *x,size_t h,size_t w,
                  const double *k,size_t kh,size_t kw,const double *want) {
    double result[1024]={0};
    assert(same_conv2d(x,h,w,k,kh,kw,result));
    for(size_t i=0;i<h*w;++i)assert(fabs(result[i]-want[i])<1e-9);
}
int main(void){
    const double image[]={1,2,3,4,5,6,7,8,9};
    const double kernel[]={1,1,1,1,1,1,1,1,1};
    const double want[]={12,21,16,27,45,33,24,39,28};
    check(image,3,3,kernel,3,3,want);
    const double small[]={1,2,3,4},small_want[]={10,10,10,10};
    check(small,2,2,kernel,3,3,small_want);
    const double one[]={5};
    check(one,1,1,kernel,3,3,one);
    const double horizontal[]={1,2,3},h_want[]={8,14,8};
    check(horizontal,1,3,horizontal,1,3,h_want);
    check(horizontal,3,1,horizontal,3,1,h_want);
    const double negative[]={-1,2,3,-4},neg_kernel[]={-2};
    const double neg_want[]={2,-4,-6,8};
    check(negative,2,2,neg_kernel,1,1,neg_want);
    const double zero[]={0},zero_want[]={0,0,0,0};
    check(small,2,2,zero,1,1,zero_want);
    double maximum[1024],result[1024],k5[25];
    for(size_t i=0;i<1024;++i)maximum[i]=1.;
    for(size_t i=0;i<25;++i)k5[i]=0.;
    assert(same_conv2d(maximum,32,32,zero,1,1,result));
    for(size_t i=0;i<1024;++i)assert(result[i]==0);
    assert(same_conv2d(maximum,32,32,k5,5,5,result));
    for(size_t i=0;i<1024;++i)assert(result[i]==0);
    const double high[]={1000},high_want[]={1000000};
    check(high,1,1,high,1,1,high_want);
    result[0]=777;
    const double bad[]={NAN},infinite[]={INFINITY},over[]={1001};
    assert(!same_conv2d(NULL,1,1,one,1,1,result));
    assert(!same_conv2d(one,1,1,NULL,1,1,result));
    assert(!same_conv2d(one,0,1,one,1,1,result));
    assert(!same_conv2d(one,1,33,one,1,1,result));
    assert(!same_conv2d(one,1,1,one,2,1,result));
    assert(!same_conv2d(one,1,1,one,1,0,result));
    assert(!same_conv2d(one,1,1,one,1,6,result));
    assert(!same_conv2d(bad,1,1,one,1,1,result));
    assert(!same_conv2d(one,1,1,infinite,1,1,result));
    assert(!same_conv2d(over,1,1,one,1,1,result));
    assert(!same_conv2d(one,1,1,one,1,1,NULL));
    double alias[]={1};
    assert(!same_conv2d(alias,1,1,one,1,1,alias));
    assert(result[0]==777);
    puts("C same convolution tests passed");
    return 0;
}
