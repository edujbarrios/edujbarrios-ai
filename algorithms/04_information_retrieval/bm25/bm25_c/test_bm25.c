#include "bm25.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    double score = -1.0;
    double expected = log1p(90.5 / 10.5) * (3.0 * 2.2 / 4.2);
    assert(bm25(100, 10, 3, 100, 100, 1.2, 0.75, &score));
    assert(fabs(score - expected) < 1e-12);
    assert(bm25(100, 10, 0, 0, 100, 1.2, 0.75, &score) && score == 0.0);
    score = -1.0;
    assert(!bm25(0, 0, 0, 0, 100, 1.2, 0.75, &score));
    assert(!bm25(100, 101, 1, 10, 100, 1.2, 0.75, &score));
    assert(!bm25(100, 10, 11, 10, 100, 1.2, 0.75, &score));
    assert(!bm25(100, 10, 1, 10, 0, 1.2, 0.75, &score));
    assert(!bm25(100, 10, 1, 10, 100, 1.2, NAN, &score));
    assert(!bm25(100, 10, 1, 10, 100, 0.0, 0.75, &score));
    assert(score == -1.0);
    puts("C BM25 tests passed");
    return 0;
}
