#include "huber_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double p[]={2,0,-2},y[]={0,1,0};
    const double boundary[]={1,-1},zero[]={0,0};
    const double invalid[]={NAN,0},extreme[]={1.7e308};
    const double opposite[]={-1.7e308};
    double value=-9,gradient[]={-9,-9,-9};
    assert(huber_loss(p,y,3,1,&value,gradient));
    assert(fabs(value-7.0/6.0)<1e-12);
    assert(fabs(gradient[0]-1.0/3.0)<1e-12);
    assert(fabs(gradient[1]+1.0/3.0)<1e-12);
    assert(fabs(gradient[2]+1.0/3.0)<1e-12);
    assert(huber_loss(boundary,zero,2,1,&value,gradient));
    assert(value==.5 && gradient[0]==.5 && gradient[1]==-.5);
    assert(huber_loss(zero,zero,2,1,&value,gradient));
    assert(value==0 && gradient[0]==0 && gradient[1]==0);
    value=-9;gradient[0]=-9;
    assert(!huber_loss(p,y,0,1,&value,gradient));
    assert(!huber_loss(p,y,3,0,&value,gradient));
    assert(!huber_loss(invalid,zero,2,1,&value,gradient));
    assert(!huber_loss(extreme,opposite,1,1,&value,gradient));
    assert(!huber_loss(p,y,3,1,NULL,gradient));
    assert(value==-9 && gradient[0]==-9);
    puts("C Huber loss tests passed");
    return 0;
}
