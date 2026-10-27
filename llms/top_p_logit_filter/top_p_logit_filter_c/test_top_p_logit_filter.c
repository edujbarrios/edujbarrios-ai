#include "top_p_logit_filter.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double uniform[]={0,0,0,0}, descending[]={2,1,0};
    const double extreme[]={-1000,1000,0};
    const double bad[]={0,NAN,0}, huge[]={0,1001,0};
    const double single[]={5};
    bool keep[4]={false,false,false,false};
    assert(top_p_logit_filter(uniform,4,0.6,keep));
    assert(keep[0] && keep[1] && keep[2] && !keep[3]);
    assert(top_p_logit_filter(uniform,4,0.1,keep));
    assert(keep[0] && !keep[1] && !keep[2] && !keep[3]);
    assert(top_p_logit_filter(uniform,4,1,keep));
    for(size_t i=0;i<4;++i) assert(keep[i]);
    assert(top_p_logit_filter(descending,3,0.5,keep));
    assert(keep[0] && !keep[1] && !keep[2]);
    assert(top_p_logit_filter(descending,3,0.8,keep));
    assert(keep[0] && keep[1] && !keep[2]);
    assert(top_p_logit_filter(extreme,3,0.9,keep));
    assert(!keep[0] && keep[1] && !keep[2]);
    assert(top_p_logit_filter(single,1,0.3,keep) && keep[0]);
    keep[0]=true;
    assert(!top_p_logit_filter(uniform,0,0.5,keep));
    assert(!top_p_logit_filter(uniform,4097,0.5,keep));
    assert(!top_p_logit_filter(uniform,4,0,keep));
    assert(!top_p_logit_filter(uniform,4,-0.1,keep));
    assert(!top_p_logit_filter(uniform,4,1.1,keep));
    assert(!top_p_logit_filter(uniform,4,NAN,keep));
    assert(!top_p_logit_filter(bad,3,0.5,keep));
    assert(!top_p_logit_filter(huge,3,0.5,keep));
    assert(!top_p_logit_filter(NULL,4,0.5,keep));
    assert(!top_p_logit_filter(uniform,4,0.5,NULL));
    assert(keep[0]);
    puts("C nucleus filtering tests passed");
    return 0;
}
