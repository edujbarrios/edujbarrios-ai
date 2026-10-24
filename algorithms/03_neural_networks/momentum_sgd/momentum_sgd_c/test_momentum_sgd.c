#include "momentum_sgd.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double theta[]={2,-3},grad[]={1,-2},zero[]={0,0};
    const double nan[]={NAN,0},huge[]={1e308,0};
    double out[]={-9,-9},velocity[]={-9,-9};
    assert(momentum_sgd(theta,grad,zero,2,.1,out,velocity));
    assert(fabs(out[0]-1.9)<1e-12 && fabs(out[1]+2.8)<1e-12);
    assert(velocity[0]==1 && velocity[1]==-2);
    double previous_theta[]={out[0],out[1]},previous_v[]={velocity[0],velocity[1]};
    assert(momentum_sgd(previous_theta,zero,previous_v,2,.1,out,velocity));
    assert(fabs(out[0]-1.81)<1e-12 && fabs(out[1]+2.62)<1e-12);
    assert(fabs(velocity[0]-.9)<1e-12 && fabs(velocity[1]+1.8)<1e-12);
    out[0]=-9;velocity[0]=-9;
    assert(!momentum_sgd(theta,grad,zero,2,0,out,velocity));
    assert(!momentum_sgd(theta,grad,zero,0,.1,out,velocity));
    assert(!momentum_sgd(theta,nan,zero,2,.1,out,velocity));
    assert(!momentum_sgd(theta,huge,zero,2,1e308,out,velocity));
    assert(!momentum_sgd(theta,grad,zero,2,.1,NULL,velocity));
    assert(out[0]==-9 && velocity[0]==-9);
    puts("C momentum SGD tests passed");
    return 0;
}
