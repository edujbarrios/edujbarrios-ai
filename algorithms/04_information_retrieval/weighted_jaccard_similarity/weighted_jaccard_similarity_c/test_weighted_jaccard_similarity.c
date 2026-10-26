#include "weighted_jaccard_similarity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1, 2, 0}, b[] = {1, 1, 3};
    const double zero[] = {0, 0}, left[] = {1, 0}, right[] = {0, 1};
    const double high[] = {1e308, 1e308}, half[] = {1e308, 0};
    const double bad[] = {-1}, nan[] = {NAN};
    double output = -9.0;
    assert(weighted_jaccard_similarity(a, b, 3, &output));
    assert(fabs(output - 1.0 / 3.0) < 1e-12);
    assert(weighted_jaccard_similarity(zero, zero, 2, &output) && output == 1);
    assert(weighted_jaccard_similarity(left, right, 2, &output) && output == 0);
    assert(weighted_jaccard_similarity(left, left, 2, &output) && output == 1);
    assert(weighted_jaccard_similarity(high, half, 2, &output) && output == 0.5);
    output = -9;
    assert(!weighted_jaccard_similarity(a, b, 0, &output));
    assert(!weighted_jaccard_similarity(NULL, b, 3, &output));
    assert(!weighted_jaccard_similarity(a, b, 3, NULL));
    assert(!weighted_jaccard_similarity(bad, zero, 1, &output));
    assert(!weighted_jaccard_similarity(nan, zero, 1, &output));
    assert(output == -9);
    puts("C weighted Jaccard tests passed");
    return 0;
}
