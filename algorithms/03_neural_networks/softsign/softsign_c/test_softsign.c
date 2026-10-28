#include "softsign.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={-2,-1,0,1,2};
    const double expected[]={-2.0/3.0,-0.5,0,0.5,2.0/3.0};
    const double high[]={1000000.0,-1000000.0};
    const double single[]={0};
    const double nan[]={NAN},inf[]={INFINITY},bad[]={1000001};
    double output[]={99,99,99,99,99};
    assert(softsign(x,5,output));
    for (size_t i=0;i<5;++i) assert(fabs(output[i]-expected[i])<1e-12);
    assert(softsign(high,2,output));
    assert(fabs(output[0]-1000000.0/1000001.0)<1e-12);
    assert(fabs(output[1]+1000000.0/1000001.0)<1e-12);
    assert(softsign(single,1,output) && output[0]==0.0);
    output[0]=99;
    assert(!softsign(nan,1,output));
    assert(!softsign(inf,1,output));
    assert(!softsign(bad,1,output));
    assert(!softsign(x,0,output));
    assert(!softsign(x,10001,output));
    assert(!softsign(NULL,1,output));
    assert(!softsign(x,5,NULL));
    assert(output[0]==99);
    puts("C Softsign tests passed");
    return 0;
}
