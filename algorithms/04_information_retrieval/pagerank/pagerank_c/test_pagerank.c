#include "pagerank.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double dangling[]={0,1,0,0};
    const double cycle[]={0,1,0,0,0,1,1,0,0};
    const double none[]={0,0,0,0};
    const double single[]={0};
    const double negative[]={0,-1,0,0};
    const double invalid[]={0,NAN,0,0};
    const double excessive[]={0,1000001,0,0};
    double result[]={-9,-9,-9};
    assert(pagerank(dangling,2,result));
    assert(fabs(result[0]-1.0/2.85)<1e-12);
    assert(fabs(result[1]-(1-1.0/2.85))<1e-12);
    assert(pagerank(cycle,3,result));
    for (size_t i=0;i<3;++i) assert(fabs(result[i]-1.0/3.0)<1e-12);
    assert(pagerank(none,2,result));
    assert(fabs(result[0]-.5)<1e-12 && fabs(result[1]-.5)<1e-12);
    assert(pagerank(single,1,result)&&fabs(result[0]-1)<1e-12);
    result[0]=-9;
    assert(!pagerank(negative,2,result));
    assert(!pagerank(invalid,2,result));
    assert(!pagerank(excessive,2,result));
    assert(!pagerank(dangling,0,result));
    assert(!pagerank(dangling,2,NULL));
    assert(result[0]==-9);
    puts("C PageRank tests passed");
    return 0;
}
