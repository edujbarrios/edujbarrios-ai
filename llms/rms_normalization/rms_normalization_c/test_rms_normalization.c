#include "rms_normalization.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={1,1}, gain[]={1,2}, zero[]={0,0};
    const double bad[]={1,NAN}, huge[]={1,1001};
    double out[2]={99,99};
    assert(rms_normalization(x,gain,2,1,out));
    assert(fabs(out[0]-1/sqrt(2.0))<1e-12);
    assert(fabs(out[1]-2/sqrt(2.0))<1e-12);
    assert(rms_normalization(zero,gain,2,1e-8,out));
    assert(out[0]==0 && out[1]==0);
    assert(rms_normalization(x,gain,1,1,out));
    assert(fabs(out[0]-1/sqrt(2.0))<1e-12);
    out[0]=99;
    assert(!rms_normalization(x,gain,0,1,out));
    assert(!rms_normalization(x,gain,1025,1,out));
    assert(!rms_normalization(x,gain,2,0,out));
    assert(!rms_normalization(x,gain,2,1e-9,out));
    assert(!rms_normalization(x,gain,2,2,out));
    assert(!rms_normalization(x,gain,2,NAN,out));
    assert(!rms_normalization(bad,gain,2,1,out));
    assert(!rms_normalization(x,bad,2,1,out));
    assert(!rms_normalization(huge,gain,2,1,out));
    assert(!rms_normalization(NULL,gain,2,1,out));
    assert(!rms_normalization(x,gain,2,1,NULL));
    assert(out[0]==99);
    puts("C RMSNorm tests passed");
    return 0;
}
