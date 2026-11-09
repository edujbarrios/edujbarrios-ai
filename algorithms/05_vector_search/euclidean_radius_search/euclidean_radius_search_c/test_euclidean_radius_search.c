#include "euclidean_radius_search.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
static void check(const double *q,const double *x,size_t n,size_t d,
                  double radius,const size_t *want,size_t expected) {
    size_t out[128]={0},count=777;
    assert(euclidean_radius_search(q,x,n,d,radius,out,&count));
    assert(count==expected);
    for(size_t i=0;i<count;++i)assert(out[i]==want[i]);
}
int main(void) {
    const double q[]={0,0},x[]={1,0,0,1,3,4,0,0,-1,0};
    const size_t one[]={3},close[]={3,0,1,4},all[]={3,0,1,4,2};
    check(q,x,5,2,0,one,1);
    check(q,x,5,2,1,close,4);
    check(q,x,5,2,5,all,5);
    check(q,x,5,2,0.99,one,1);
    const double far[]={1,0};
    check(q,far,1,2,0.5,NULL,0);
    const double q1[]={2},duplicates[]={2,2,3};
    const size_t dup[]={0,1};
    check(q1,duplicates,3,1,0,dup,2);
    double high[32],low[32];
    for(size_t j=0;j<32;++j){high[j]=1000.;low[j]=-1000.;}
    const size_t idx[]={0};
    check(high,low,1,32,10000.,idx,1);
    double zero_q[32]={0},zero_x[4096]={0};
    size_t entire[128];
    for(size_t i=0;i<128;++i)entire[i]=i;
    check(zero_q,zero_x,128,32,0,entire,128);
    size_t out[128]={777},count=888;
    const double invalid[]={NAN},over[]={1001},infinity[]={INFINITY};
    assert(!euclidean_radius_search(NULL,x,5,2,1,out,&count));
    assert(!euclidean_radius_search(q,NULL,5,2,1,out,&count));
    assert(!euclidean_radius_search(q,x,0,2,1,out,&count));
    assert(!euclidean_radius_search(q,x,129,2,1,out,&count));
    assert(!euclidean_radius_search(q,x,1,0,1,out,&count));
    assert(!euclidean_radius_search(q,x,1,33,1,out,&count));
    assert(!euclidean_radius_search(q,x,5,2,-1,out,&count));
    assert(!euclidean_radius_search(q,x,5,2,NAN,out,&count));
    assert(!euclidean_radius_search(q,x,5,2,10001,out,&count));
    assert(!euclidean_radius_search(invalid,far,1,1,1,out,&count));
    assert(!euclidean_radius_search(over,far,1,1,1,out,&count));
    assert(!euclidean_radius_search(q,infinity,1,1,1,out,&count));
    assert(!euclidean_radius_search(q,x,5,2,1,NULL,&count));
    assert(!euclidean_radius_search(q,x,5,2,1,out,NULL));
    assert(!euclidean_radius_search(q,x,5,2,1,out,out));
    assert(out[0]==777 && count==888);
    puts("C Euclidean radius search tests passed");
    return 0;
}
