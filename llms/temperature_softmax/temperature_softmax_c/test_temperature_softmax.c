#include "temperature_softmax.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logits[]={0,2}, shifted[]={998,1000};
    const double uniform[]={3,3,3}, extreme[]={-1000,1000};
    const double bad[]={NAN,2}, oversized[]={1001,2};
    double out[3]={99,99,99}, reference=exp(1)/(1+exp(1));
    assert(temperature_softmax(logits,2,2,out));
    assert(fabs(out[0]-(1-reference))<1e-12 && fabs(out[1]-reference)<1e-12);
    assert(temperature_softmax(shifted,2,2,out));
    assert(fabs(out[0]-(1-reference))<1e-12);
    assert(temperature_softmax(uniform,3,100,out));
    for(size_t i=0;i<3;++i) assert(fabs(out[i]-1.0/3.0)<1e-12);
    assert(temperature_softmax(extreme,2,0.01,out));
    assert(out[0]==0 && out[1]==1);
    assert(temperature_softmax(logits,1,1,out) && out[0]==1);
    out[0]=99;
    assert(!temperature_softmax(logits,0,1,out));
    assert(!temperature_softmax(logits,4097,1,out));
    assert(!temperature_softmax(logits,2,0,out));
    assert(!temperature_softmax(logits,2,0.001,out));
    assert(!temperature_softmax(logits,2,101,out));
    assert(!temperature_softmax(logits,2,NAN,out));
    assert(!temperature_softmax(bad,2,1,out));
    assert(!temperature_softmax(oversized,2,1,out));
    assert(!temperature_softmax(NULL,2,1,out));
    assert(!temperature_softmax(logits,2,1,NULL));
    assert(out[0]==99);
    puts("C temperature softmax tests passed");
    return 0;
}
