#include "swish.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={0,1,-1,1000,-1000};
    const double bad[]={1,NAN};
    const double inf[]={INFINITY};
    double out[]={-9,-9,-9,-9,-9},grad[]={-9,-9,-9,-9,-9};
    const double s=1.0/(1.0+exp(-1.0));
    assert(swish(x,5,out,grad));
    assert(out[0]==0 && grad[0]==0.5);
    assert(fabs(out[1]-s)<1e-12);
    assert(fabs(grad[1]-(s+s*(1-s)))<1e-12);
    assert(fabs(out[2]-(s-1))<1e-12);
    assert(fabs(grad[2]-(1-s)*(1-s))<1e-12);
    assert(out[3]==1000 && grad[3]==1);
    assert(out[4]==0 && grad[4]==0);
    out[0]=-9;grad[0]=-9;
    assert(!swish(x,0,out,grad));
    assert(!swish(bad,2,out,grad));
    assert(!swish(inf,1,out,grad));
    assert(!swish(NULL,5,out,grad));
    assert(!swish(x,5,NULL,grad));
    assert(out[0]==-9 && grad[0]==-9);
    puts("C Swish tests passed");
    return 0;
}
