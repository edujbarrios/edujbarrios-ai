#include "f1_score.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const uint8_t actual[]={0,0,1,1,1};
    const uint8_t predicted[]={0,1,1,0,1};
    const uint8_t zeros[]={0,0};
    const uint8_t ones[]={1,1};
    const uint8_t opposite[]={0,1};
    const uint8_t invalid[]={0,2};
    double score=-9;
    assert(f1_score(actual,predicted,5,&score));
    assert(fabs(score-2.0/3.0)<1e-12);
    assert(f1_score(zeros,zeros,2,&score) && score==0.0);
    assert(f1_score(ones,ones,2,&score) && score==1.0);
    assert(f1_score(ones,opposite,2,&score) && fabs(score-2.0/3.0)<1e-12);
    score=-9;
    assert(!f1_score(actual,predicted,0,&score));
    assert(!f1_score(zeros,invalid,2,&score));
    assert(!f1_score(NULL,predicted,5,&score));
    assert(!f1_score(actual,NULL,5,&score));
    assert(!f1_score(actual,predicted,5,NULL));
    assert(score==-9);
    puts("C F1 score tests passed");
    return 0;
}
