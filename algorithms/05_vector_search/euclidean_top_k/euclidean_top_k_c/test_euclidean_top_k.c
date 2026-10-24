#include "euclidean_top_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double query[]={0,0};
    const double rows[]={1,0,0,1,3,4,0,0,-1,0};
    const double duplicates[]={1,0,1,0,0,0};
    const double invalid[]={0,0,INFINITY,1};
    const double far[]={1e200,0,2e200,0};
    const double extreme_q[]={1.7e308};
    const double extreme_rows[]={-1.7e308};
    size_t indices[]={99,99,99,99,99};
    assert(euclidean_top_k(query,2,rows,5,5,indices));
    assert(indices[0]==3 && indices[1]==0 && indices[2]==1 &&
           indices[3]==4 && indices[4]==2);
    assert(euclidean_top_k(query,2,duplicates,3,3,indices));
    assert(indices[0]==2 && indices[1]==0 && indices[2]==1);
    assert(euclidean_top_k(query,2,far,2,2,indices));
    assert(indices[0]==0 && indices[1]==1);
    indices[0]=99;
    assert(!euclidean_top_k(query,2,invalid,2,1,indices));
    assert(!euclidean_top_k(extreme_q,1,extreme_rows,1,1,indices));
    assert(!euclidean_top_k(query,2,rows,5,0,indices));
    assert(!euclidean_top_k(query,2,rows,5,6,indices));
    assert(!euclidean_top_k(query,0,rows,5,1,indices));
    assert(!euclidean_top_k(query,2,NULL,5,1,indices));
    assert(indices[0]==99);
    puts("C Euclidean Top-K tests passed");
    return 0;
}
