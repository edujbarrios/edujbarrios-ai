#include "temperature_softmax.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logits[]={0,1.0986122886681098};
    const double equal[]={10,10};
    const double large[]={1000,1001};
    const double huge[]={-1e308,1e308};
    const double invalid[]={NAN,1};
    double out[]={-9,-9};
    assert(temperature_softmax(logits,2,1,out));
    assert(fabs(out[0]-.25)<1e-12 && fabs(out[1]-.75)<1e-12);
    assert(temperature_softmax(logits,2,.5,out));
    assert(fabs(out[0]-.1)<1e-12 && fabs(out[1]-.9)<1e-12);
    assert(temperature_softmax(logits,2,2,out));
    assert(fabs(out[0]-1/(1+sqrt(3.0)))<1e-12);
    assert(temperature_softmax(equal,2,1,out));
    assert(out[0]==.5 && out[1]==.5);
    assert(temperature_softmax(large,2,1,out));
    assert(fabs(out[0]-1/(1+exp(1.0)))<1e-12);
    out[0]=-9;
    assert(!temperature_softmax(logits,2,0,out));
    assert(!temperature_softmax(logits,2,-1,out));
    assert(!temperature_softmax(invalid,2,1,out));
    assert(!temperature_softmax(huge,2,1,out));
    assert(!temperature_softmax(logits,2,NAN,out));
    assert(!temperature_softmax(logits,0,1,out));
    assert(!temperature_softmax(logits,2,1,NULL));
    assert(out[0]==-9);
    puts("C temperature-softmax tests passed");
    return 0;
}
