#include "shannon_entropy.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double certain[]={1,0};
    const double half[]={.5,.5};
    const double quarters[]={.25,.25,.25,.25};
    const double uneven[]={.75,.25};
    const double malformed[]={.2,.2};
    const double negative[]={1.1,-.1};
    const double nan[]={NAN,1};
    const double singleton[]={1};
    double out=-9;
    assert(shannon_entropy(certain,2,&out) && out==0);
    assert(shannon_entropy(half,2,&out) && fabs(out-1)<1e-12);
    assert(shannon_entropy(quarters,4,&out) && fabs(out-2)<1e-12);
    assert(shannon_entropy(uneven,2,&out));
    assert(fabs(out-.8112781244591328)<1e-12);
    assert(shannon_entropy(singleton,1,&out) && out==0);
    out=-9;
    assert(!shannon_entropy(malformed,2,&out));
    assert(!shannon_entropy(negative,2,&out));
    assert(!shannon_entropy(nan,2,&out));
    assert(!shannon_entropy(half,0,&out));
    assert(!shannon_entropy(half,2,NULL));
    assert(out==-9);
    puts("C Shannon entropy tests passed");
    return 0;
}
