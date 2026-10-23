#include "recall_at_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint8_t labels[] = {1,0,1,0};
    const uint8_t zeros[] = {0,0};
    const uint8_t bad[] = {1,0,2};
    double out = -1.0;
    assert(recall_at_k(labels,4,3,3,&out) && fabs(out-2.0/3.0)<1e-12);
    assert(recall_at_k(labels,4,1,3,&out) && fabs(out-1.0/3.0)<1e-12);
    assert(recall_at_k(zeros,2,2,5,&out) && out == 0.0);
    out = -1.0;
    assert(!recall_at_k(labels,4,0,3,&out));
    assert(!recall_at_k(labels,4,5,3,&out));
    assert(!recall_at_k(labels,4,3,0,&out));
    assert(!recall_at_k(labels,4,3,1,&out));
    assert(!recall_at_k(bad,3,1,3,&out));
    assert(!recall_at_k(NULL,4,3,3,&out));
    assert(!recall_at_k(labels,4,3,3,NULL));
    assert(out == -1.0);
    puts("C recall at k tests passed");
    return 0;
}
