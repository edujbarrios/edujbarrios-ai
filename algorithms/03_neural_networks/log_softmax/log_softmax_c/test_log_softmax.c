#include "log_softmax.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double equal[]={0,0};
    const double unequal[]={0,log(2.0)};
    const double shifted[]={100,100+log(2.0)};
    const double extreme[]={1000000,-1000000};
    const double single[]={5};
    const double bad[]={0,NAN},inf[]={0,INFINITY};
    const double high[]={0,1000001};
    double out[]={99,99};
    assert(log_softmax(equal,2,out));
    assert(fabs(out[0]+log(2.0))<1e-12 && out[0]==out[1]);
    assert(log_softmax(unequal,2,out));
    assert(fabs(out[0]+log(3.0))<1e-12);
    assert(fabs(out[1]-(log(2.0)-log(3.0)))<1e-12);
    assert(fabs(exp(out[0])+exp(out[1])-1.0)<1e-12);
    double reference0=out[0],reference1=out[1];
    assert(log_softmax(shifted,2,out));
    assert(fabs(out[0]-reference0)<1e-12 && fabs(out[1]-reference1)<1e-12);
    assert(log_softmax(extreme,2,out));
    assert(out[0]==0 && out[1]==-2000000);
    assert(log_softmax(single,1,out) && out[0]==0);
    out[0]=99;
    assert(!log_softmax(bad,2,out));
    assert(!log_softmax(inf,2,out));
    assert(!log_softmax(high,2,out));
    assert(!log_softmax(equal,0,out));
    assert(!log_softmax(equal,10001,out));
    assert(!log_softmax(NULL,2,out));
    assert(!log_softmax(equal,2,NULL));
    assert(out[0]==99);
    puts("C log-softmax tests passed");
    return 0;
}
