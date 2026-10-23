#include "jaccard_similarity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const int32_t a[] = {1,1,2,3};
    const int32_t b[] = {2,3,4};
    const int32_t c[] = {-5,3};
    const int32_t d[] = {3,-5};
    const int32_t extreme[] = {INT32_MIN,INT32_MAX};
    double score = -1.0;
    assert(jaccard_similarity(a,4,b,3,&score) && fabs(score-0.5)<1e-12);
    assert(jaccard_similarity(NULL,0,NULL,0,&score) && score==1.0);
    assert(jaccard_similarity(NULL,0,b,3,&score) && score==0.0);
    assert(jaccard_similarity(c,2,d,2,&score) && score==1.0);
    assert(jaccard_similarity(extreme,2,extreme,2,&score) && score==1.0);
    score = -1.0;
    assert(!jaccard_similarity(NULL,1,b,3,&score));
    assert(!jaccard_similarity(a,4,NULL,1,&score));
    assert(!jaccard_similarity(a,4,b,3,NULL));
    assert(score == -1.0);
    puts("C Jaccard tests passed");
    return 0;
}
