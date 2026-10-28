#include "spearman_rank_correlation.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {1,2,3};
    const double positive[] = {4,5,6};
    const double negative[] = {6,5,4};
    const double unusual[] = {1,3,2};
    const double ties_x[] = {1,1,2,3};
    const double ties_y[] = {1,2,2,3};
    const double constant[] = {7,7,7};
    const double bad[] = {1,NAN,3};
    const double large[] = {1,1000001,3};
    double out = 99.0;
    assert(spearman_rank_correlation(x,positive,3,&out) && fabs(out-1)<1e-12);
    assert(spearman_rank_correlation(x,negative,3,&out) && fabs(out+1)<1e-12);
    assert(spearman_rank_correlation(x,unusual,3,&out) && fabs(out-0.5)<1e-12);
    assert(spearman_rank_correlation(ties_x,ties_y,4,&out) && fabs(out-5.0/6.0)<1e-12);
    out = 99.0;
    assert(!spearman_rank_correlation(x,constant,3,&out));
    assert(!spearman_rank_correlation(x,bad,3,&out));
    assert(!spearman_rank_correlation(x,large,3,&out));
    assert(!spearman_rank_correlation(x,positive,1,&out));
    assert(!spearman_rank_correlation(x,positive,129,&out));
    assert(!spearman_rank_correlation(NULL,positive,3,&out));
    assert(!spearman_rank_correlation(x,positive,3,NULL));
    assert(out==99.0);
    puts("C Spearman rank correlation tests passed");
    return 0;
}
