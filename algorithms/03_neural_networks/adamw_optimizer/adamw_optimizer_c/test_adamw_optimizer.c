#include "adamw_optimizer.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double theta[]={2,-3},g[]={1,-2},zero[]={0,0};
    const double bad_v[]={-1,0},huge[]={1e308,0};
    double out[]={-9,-9},m[]={-9,-9},v[]={-9,-9};
    assert(adamw_optimizer(theta,g,zero,zero,2,1,.1,out,m,v));
    assert(fabs(m[0]-.1)<1e-12 && fabs(m[1]+.2)<1e-12);
    assert(fabs(v[0]-.001)<1e-12 && fabs(v[1]-.004)<1e-12);
    assert(fabs(out[0]-(2-.1*(1/(1+1e-8)+.02)))<1e-12);
    assert(fabs(out[1]-(-3-.1*(-2/(2+1e-8)-.03)))<1e-12);
    double previous_theta[]={out[0],out[1]},previous_m[]={m[0],m[1]};
    double previous_v[]={v[0],v[1]};
    assert(adamw_optimizer(previous_theta,zero,previous_m,previous_v,
                           2,2,.1,out,m,v));
    assert(fabs(m[0]-.09)<1e-12 && fabs(m[1]+.18)<1e-12);
    assert(fabs(v[0]-.000999)<1e-12 && fabs(v[1]-.003996)<1e-12);
    assert(out[0]<previous_theta[0] && out[1]>previous_theta[1]);
    out[0]=-9;m[0]=-9;v[0]=-9;
    assert(!adamw_optimizer(theta,g,zero,zero,2,0,.1,out,m,v));
    assert(!adamw_optimizer(theta,g,zero,zero,2,1,0,out,m,v));
    assert(!adamw_optimizer(theta,g,zero,bad_v,2,1,.1,out,m,v));
    assert(!adamw_optimizer(theta,huge,zero,zero,2,1,.1,out,m,v));
    assert(!adamw_optimizer(theta,g,zero,zero,0,1,.1,out,m,v));
    assert(!adamw_optimizer(theta,g,zero,zero,2,1,.1,NULL,m,v));
    assert(out[0]==-9 && m[0]==-9 && v[0]==-9);
    puts("C AdamW tests passed");
    return 0;
}
