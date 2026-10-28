#include "typical_sampling_filter.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double uniform[]={0,0,0,0}, distinct[]={2,1,0};
    const double extreme[]={-1000,1000,0}, single[]={5};
    const double bad[]={0,NAN}, high[]={0,1001};
    bool keep[4]={false,false,false,false};
    assert(typical_sampling_filter(uniform,4,0.6,keep));
    assert(keep[0] && keep[1] && keep[2] && !keep[3]);
    assert(typical_sampling_filter(distinct,3,0.8,keep));
    assert(keep[0] && keep[1] && !keep[2]);
    assert(typical_sampling_filter(uniform,4,1,keep));
    for(size_t i=0;i<4;++i) assert(keep[i]);
    assert(typical_sampling_filter(extreme,3,0.8,keep));
    assert(!keep[0] && keep[1] && !keep[2]);
    assert(typical_sampling_filter(single,1,0.5,keep) && keep[0]);
    keep[0]=true;
    assert(!typical_sampling_filter(uniform,0,0.5,keep));
    assert(!typical_sampling_filter(uniform,4097,0.5,keep));
    assert(!typical_sampling_filter(uniform,4,0,keep));
    assert(!typical_sampling_filter(uniform,4,-1,keep));
    assert(!typical_sampling_filter(uniform,4,1.1,keep));
    assert(!typical_sampling_filter(uniform,4,NAN,keep));
    assert(!typical_sampling_filter(bad,2,0.5,keep));
    assert(!typical_sampling_filter(high,2,0.5,keep));
    assert(!typical_sampling_filter(NULL,4,0.5,keep));
    assert(!typical_sampling_filter(uniform,4,0.5,NULL));
    assert(keep[0]);
    puts("C locally typical filter tests passed");
    return 0;
}
