#include "top_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double scores[] = {0.8, 0.9, 0.9, -1.0};
    const double tied[] = {1.0, 1.0, 1.0};
    const double bad[] = {0.1, NAN};
    const double inf[] = {INFINITY};
    size_t out[] = {99,99,99,99};
    assert(top_k(scores,4,3,out));
    assert(out[0]==1 && out[1]==2 && out[2]==0);
    assert(top_k(tied,3,2,out));
    assert(out[0]==0 && out[1]==1);
    out[0]=99;
    assert(!top_k(scores,4,0,out));
    assert(!top_k(scores,4,5,out));
    assert(!top_k(bad,2,1,out));
    assert(!top_k(inf,1,1,out));
    assert(!top_k(NULL,4,2,out));
    assert(!top_k(scores,4,2,NULL));
    assert(out[0]==99);
    puts("C Top-K tests passed");
    return 0;
}
