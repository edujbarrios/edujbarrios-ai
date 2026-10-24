#include "brier_score.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double predictions[]={.1,.8,.6,.2};
    const uint8_t labels[]={0,1,1,0};
    const double perfect[]={0,1},wrong[]={1,0},half[]={.5,.5};
    const uint8_t two[]={0,1},bad_labels[]={0,2};
    const double invalid[]={-.1,.5},huge[]={1.1,.5},nan[]={NAN,.5};
    double score=-9;
    assert(brier_score(predictions,labels,4,&score));
    assert(fabs(score-.0625)<1e-12);
    assert(brier_score(perfect,two,2,&score) && score==0.0);
    assert(brier_score(wrong,two,2,&score) && score==1.0);
    assert(brier_score(half,two,2,&score) && score==.25);
    assert(brier_score(half,two,1,&score) && score==.25);
    score=-9;
    assert(!brier_score(predictions,labels,0,&score));
    assert(!brier_score(invalid,two,2,&score));
    assert(!brier_score(huge,two,2,&score));
    assert(!brier_score(nan,two,2,&score));
    assert(!brier_score(perfect,bad_labels,2,&score));
    assert(!brier_score(predictions,labels,4,NULL));
    assert(!brier_score(NULL,labels,4,&score));
    assert(score==-9);
    puts("C Brier score tests passed");
    return 0;
}
