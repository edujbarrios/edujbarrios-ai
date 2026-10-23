#include "cosine_similarity.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1.0, 2.0};
    const double b[] = {2.0, 4.0};
    const double orth[] = {-2.0, 1.0};
    const double neg[] = {-1.0, -2.0};
    const double zero[] = {0.0, 0.0};
    const double huge[] = {1e308, 1e308};
    const double tiny[] = {1e-308, 1e-308};
    const double nan_value[] = {NAN};
    const double inf_value[] = {INFINITY};
    double result = -10.0;
    assert(cosine_similarity(a, b, 2, &result) && fabs(result - 1.0) < 1e-12);
    assert(cosine_similarity(a, orth, 2, &result) && fabs(result) < 1e-12);
    assert(cosine_similarity(a, neg, 2, &result) && fabs(result + 1.0) < 1e-12);
    assert(cosine_similarity(huge, tiny, 2, &result) && fabs(result - 1.0) < 1e-12);
    result = -10.0;
    assert(!cosine_similarity(a, zero, 2, &result) && result == -10.0);
    assert(!cosine_similarity(a, b, 0, &result) && result == -10.0);
    assert(!cosine_similarity(nan_value, b, 1, &result) && result == -10.0);
    assert(!cosine_similarity(inf_value, b, 1, &result) && result == -10.0);
    assert(!cosine_similarity(a, b, 2, NULL));
    puts("C cosine similarity tests passed");
    return 0;
}
