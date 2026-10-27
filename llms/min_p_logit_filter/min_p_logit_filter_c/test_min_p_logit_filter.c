#include "min_p_logit_filter.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={0,-1,-2}, ties[]={5,5,0};
    const double far[]={-1000,1000}, one[]={-5};
    const double nan[]={1,NAN}, big[]={1,1001};
    bool keep[3]={false,false,false};
    assert(min_p_logit_filter(x,3,0.3,keep));
    assert(keep[0] && keep[1] && !keep[2]);
    assert(min_p_logit_filter(ties,3,1,keep));
    assert(keep[0] && keep[1] && !keep[2]);
    assert(min_p_logit_filter(far,2,0,keep));
    assert(keep[0] && keep[1]);
    assert(min_p_logit_filter(far,2,0.5,keep));
    assert(!keep[0] && keep[1]);
    assert(min_p_logit_filter(one,1,1,keep) && keep[0]);
    keep[0]=false;
    assert(!min_p_logit_filter(x,0,0.5,keep));
    assert(!min_p_logit_filter(x,4097,0.5,keep));
    assert(!min_p_logit_filter(x,3,-0.1,keep));
    assert(!min_p_logit_filter(x,3,1.1,keep));
    assert(!min_p_logit_filter(x,3,NAN,keep));
    assert(!min_p_logit_filter(nan,2,0.5,keep));
    assert(!min_p_logit_filter(big,2,0.5,keep));
    assert(!min_p_logit_filter(NULL,3,0.5,keep));
    assert(!min_p_logit_filter(x,3,0.5,NULL));
    assert(!keep[0]);
    puts("C min-p filter tests passed");
    return 0;
}
