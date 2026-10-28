#include "elu.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={-1,0,1},expected[]={-0.6321205588285577,0,1};
    const double extremes[]={-1000000,1000000},near[]={-1e-9,1e-9};
    const double bad[]={NAN},inf[]={INFINITY},over[]={1000001};
    double result[]={99,99,99};
    assert(elu(x,3,result));
    for(size_t i=0;i<3;++i) assert(fabs(result[i]-expected[i])<1e-12);
    assert(elu(extremes,2,result));
    assert(result[0]==-1 && result[1]==1000000);
    assert(elu(near,2,result));
    assert(fabs(result[0]-expm1(-1e-9))<1e-15);
    assert(result[1]==1e-9);
    result[0]=99;
    assert(!elu(bad,1,result));
    assert(!elu(inf,1,result));
    assert(!elu(over,1,result));
    assert(!elu(x,0,result));
    assert(!elu(x,10001,result));
    assert(!elu(NULL,1,result));
    assert(!elu(x,3,NULL));
    assert(result[0]==99);
    puts("C ELU tests passed");
    return 0;
}
