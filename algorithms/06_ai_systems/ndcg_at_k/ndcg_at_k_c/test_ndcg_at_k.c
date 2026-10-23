#include "ndcg_at_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const uint8_t ideal[] = {3,2,1,0};
    const uint8_t unordered[] = {3,2,0,1};
    const uint8_t zeros[] = {0,0,0};
    const uint8_t invalid[] = {3,1,21};
    double out = -1.0;
    assert(ndcg_at_k(ideal,4,3,&out) && fabs(out - 1.0) < 1e-12);
    assert(ndcg_at_k(unordered,4,3,&out) && out > 0.9 && out < 1.0);
    assert(ndcg_at_k(zeros,3,2,&out) && out == 0.0);
    out = -1.0;
    assert(!ndcg_at_k(invalid,3,1,&out));
    assert(!ndcg_at_k(ideal,4,0,&out));
    assert(!ndcg_at_k(ideal,4,5,&out));
    assert(!ndcg_at_k(NULL,4,2,&out));
    assert(!ndcg_at_k(ideal,4,2,NULL));
    assert(out == -1.0);
    puts("C NDCG@K tests passed");
    return 0;
}
