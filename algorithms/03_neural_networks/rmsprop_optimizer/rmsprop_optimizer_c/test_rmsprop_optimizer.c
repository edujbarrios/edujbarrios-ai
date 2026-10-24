#include "rmsprop_optimizer.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double theta[]={2,-3},g[]={1,-2},zeros[]={0,0};
    const double neg[]={-1,0},bad[]={NAN,0},huge[]={1e308,0};
    double next[]={-9,-9},v[]={-9,-9};
    assert(rmsprop_optimizer(theta,g,zeros,2,.1,next,v));
    assert(fabs(v[0]-.1)<1e-12 && fabs(v[1]-.4)<1e-12);
    assert(fabs(next[0]-(2-.1/(sqrt(.1)+1e-8)))<1e-12);
    assert(fabs(next[1]-(-3+.2/(sqrt(.4)+1e-8)))<1e-12);
    double second_theta[]={next[0],next[1]},previous[]={v[0],v[1]};
    assert(rmsprop_optimizer(second_theta,zeros,previous,2,.1,next,v));
    assert(fabs(v[0]-.09)<1e-12 && fabs(v[1]-.36)<1e-12);
    assert(next[0]==second_theta[0] && next[1]==second_theta[1]);
    next[0]=-9;v[0]=-9;
    assert(!rmsprop_optimizer(theta,g,neg,2,.1,next,v));
    assert(!rmsprop_optimizer(theta,bad,zeros,2,.1,next,v));
    assert(!rmsprop_optimizer(theta,huge,zeros,2,.1,next,v));
    assert(!rmsprop_optimizer(theta,g,zeros,2,0,next,v));
    assert(!rmsprop_optimizer(theta,g,zeros,0,.1,next,v));
    assert(!rmsprop_optimizer(theta,g,zeros,2,.1,NULL,v));
    assert(next[0]==-9 && v[0]==-9);
    puts("C RMSProp tests passed");
    return 0;
}
