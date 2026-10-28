#include "entropy_impurity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint32_t pure[]={5,0},balanced[]={1,1},four[]={1,1,1,1};
    const uint32_t uneven[]={3,1},one[]={1000000},zeros[]={0,0};
    const uint32_t invalid[]={0,1000001},maximal[]={1000000,1000000};
    double out=99.0;
    assert(entropy_impurity(pure,2,&out) && out==0.0);
    assert(entropy_impurity(balanced,2,&out) && fabs(out-1.0)<1e-12);
    assert(entropy_impurity(four,4,&out) && fabs(out-2.0)<1e-12);
    assert(entropy_impurity(uneven,2,&out) &&
           fabs(out-0.8112781244591328)<1e-12);
    assert(entropy_impurity(one,1,&out) && out==0.0);
    assert(entropy_impurity(maximal,2,&out) && fabs(out-1.0)<1e-12);
    out=99.0;
    assert(!entropy_impurity(zeros,2,&out));
    assert(!entropy_impurity(invalid,2,&out));
    assert(!entropy_impurity(one,0,&out));
    assert(!entropy_impurity(one,129,&out));
    assert(!entropy_impurity(NULL,2,&out));
    assert(!entropy_impurity(one,1,NULL));
    assert(out==99.0);
    puts("C entropy impurity tests passed");
    return 0;
}
