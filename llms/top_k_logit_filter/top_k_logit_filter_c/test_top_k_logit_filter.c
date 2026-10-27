#include "top_k_logit_filter.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double values[]={2,5,5,-1};
    const double bad[]={2,NAN,5,-1}, big[]={2,1001,5,-1};
    const double negative[]={-2,-1,-3,-1};
    const double single[]={-3};
    bool keep[4]={false,false,false,false};
    assert(top_k_logit_filter(values,4,2,keep));
    assert(!keep[0] && keep[1] && keep[2] && !keep[3]);
    assert(top_k_logit_filter(values,4,1,keep));
    assert(!keep[0] && keep[1] && !keep[2] && !keep[3]);
    assert(top_k_logit_filter(values,4,4,keep));
    for (size_t i=0;i<4;++i) assert(keep[i]);
    assert(top_k_logit_filter(negative,4,2,keep));
    assert(!keep[0] && keep[1] && !keep[2] && keep[3]);
    assert(top_k_logit_filter(single,1,1,keep) && keep[0]);
    keep[0]=false;
    assert(!top_k_logit_filter(values,0,1,keep));
    assert(!top_k_logit_filter(values,4097,1,keep));
    assert(!top_k_logit_filter(values,4,0,keep));
    assert(!top_k_logit_filter(values,4,5,keep));
    assert(!top_k_logit_filter(bad,4,2,keep));
    assert(!top_k_logit_filter(big,4,2,keep));
    assert(!top_k_logit_filter(NULL,4,2,keep));
    assert(!top_k_logit_filter(values,4,2,NULL));
    assert(!keep[0]);
    puts("C top-k filtering tests passed");
    return 0;
}
