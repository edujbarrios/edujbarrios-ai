#include "hits_hubs_authorities.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double star[]={0,1,1,0,0,0,0,0,0};
    const double cycle[]={0,1,0,0,0,1,1,0,0};
    const double none[]={0,0,0,0};
    const double single[]={1};
    const double negative[]={0,-1,0,0};
    const double invalid[]={0,NAN,0,0};
    const double excessive[]={0,2,0,0};
    double a[]={-9,-9,-9},h[]={-9,-9,-9};
    assert(hits_hubs_authorities(star,3,a,h));
    assert(a[0]==0.0 && fabs(a[1]-1/sqrt(2.0))<1e-12);
    assert(fabs(a[2]-1/sqrt(2.0))<1e-12);
    assert(h[0]==1.0 && h[1]==0.0 && h[2]==0.0);
    assert(hits_hubs_authorities(cycle,3,a,h));
    for (size_t i=0;i<3;++i) {
        assert(fabs(a[i]-1/sqrt(3.0))<1e-12);
        assert(fabs(h[i]-1/sqrt(3.0))<1e-12);
    }
    assert(hits_hubs_authorities(none,2,a,h));
    assert(a[0]==0 && a[1]==0 && h[0]==0 && h[1]==0);
    assert(hits_hubs_authorities(single,1,a,h) && a[0]==1 && h[0]==1);
    a[0]=-9;h[0]=-9;
    assert(!hits_hubs_authorities(negative,2,a,h));
    assert(!hits_hubs_authorities(invalid,2,a,h));
    assert(!hits_hubs_authorities(excessive,2,a,h));
    assert(!hits_hubs_authorities(none,0,a,h));
    assert(!hits_hubs_authorities(none,2,NULL,h));
    assert(a[0]==-9 && h[0]==-9);
    puts("C HITS tests passed");
    return 0;
}
