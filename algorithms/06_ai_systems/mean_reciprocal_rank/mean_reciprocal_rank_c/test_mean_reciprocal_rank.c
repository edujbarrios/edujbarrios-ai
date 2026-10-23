#include "mean_reciprocal_rank.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const size_t ranks[] = {1,2,0,4};
    const size_t absent[] = {0,0,0};
    const size_t first[] = {1};
    double out = -1.0;
    assert(mean_reciprocal_rank(ranks,4,&out) && fabs(out-0.4375)<1e-12);
    assert(mean_reciprocal_rank(absent,3,&out) && out==0.0);
    assert(mean_reciprocal_rank(first,1,&out) && out==1.0);
    out=-1.0;
    assert(!mean_reciprocal_rank(ranks,0,&out));
    assert(!mean_reciprocal_rank(NULL,4,&out));
    assert(!mean_reciprocal_rank(ranks,4,NULL));
    assert(out==-1.0);
    puts("C mean reciprocal rank tests passed");
    return 0;
}
