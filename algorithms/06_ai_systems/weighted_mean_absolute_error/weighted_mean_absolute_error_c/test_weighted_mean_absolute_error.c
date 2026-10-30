#include "weighted_mean_absolute_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double y[] = {1,3}, p[] = {2,1}, w[] = {1,3};
    const double zero[] = {0,0}, partial[] = {0,2}, bad[] = {1,NAN};
    const double neg[] = {1,-1}, huge[] = {1,1000001};
    double score = -1;
    assert(weighted_mean_absolute_error(y,p,w,2,&score) && fabs(score-1.75)<1e-12);
    assert(weighted_mean_absolute_error(y,y,w,2,&score) && score==0);
    assert(weighted_mean_absolute_error(y,p,partial,2,&score) && score==2);
    assert(weighted_mean_absolute_error(y,p,w,1,&score) && score==1);
    score=-1;
    assert(!weighted_mean_absolute_error(y,p,zero,2,&score) && score==-1);
    assert(!weighted_mean_absolute_error(y,p,w,0,&score) && score==-1);
    assert(!weighted_mean_absolute_error(y,bad,w,2,&score) && score==-1);
    assert(!weighted_mean_absolute_error(y,p,neg,2,&score) && score==-1);
    assert(!weighted_mean_absolute_error(y,p,huge,2,&score) && score==-1);
    assert(!weighted_mean_absolute_error(NULL,p,w,2,&score) && score==-1);
    assert(!weighted_mean_absolute_error(y,p,w,2,NULL));
    puts("C weighted MAE tests passed");
    return 0;
}
