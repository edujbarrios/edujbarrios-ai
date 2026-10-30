#include "weighted_brier_score.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double y[] = {0,1}, p[] = {0.5,0}, w[] = {1,3};
    const double zeros[] = {0,0}, partial[] = {0,1};
    const double invalid_y[] = {0,2}, invalid_p[] = {0.5,1.1};
    const double bad_w[] = {1,NAN}, negative_w[] = {1,-1};
    double score = -1;
    assert(weighted_brier_score(y,p,w,2,&score) && score == 0.8125);
    assert(weighted_brier_score(y,y,w,2,&score) && score == 0.0);
    assert(weighted_brier_score(y,p,partial,2,&score) && score == 1.0);
    assert(weighted_brier_score(y,p,w,1,&score) && score == 0.25);
    score = -1;
    assert(!weighted_brier_score(y,p,zeros,2,&score) && score == -1);
    assert(!weighted_brier_score(y,p,w,0,&score) && score == -1);
    assert(!weighted_brier_score(invalid_y,p,w,2,&score) && score == -1);
    assert(!weighted_brier_score(y,invalid_p,w,2,&score) && score == -1);
    assert(!weighted_brier_score(y,p,bad_w,2,&score) && score == -1);
    assert(!weighted_brier_score(y,p,negative_w,2,&score) && score == -1);
    assert(!weighted_brier_score(NULL,p,w,2,&score) && score == -1);
    assert(!weighted_brier_score(y,p,w,2,NULL));
    puts("C weighted Brier tests passed");
    return 0;
}
