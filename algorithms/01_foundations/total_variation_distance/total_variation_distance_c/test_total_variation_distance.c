#include "total_variation_distance.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1, 0}, b[] = {0, 1}, c[] = {0.25, 0.75};
    const double wrong[] = {0.4, 0.4}, bad[] = {1.1, -0.1}, nan[] = {NAN, 0};
    double score = -9.0;
    assert(total_variation_distance(a, a, 2, &score) && score == 0.0);
    assert(total_variation_distance(a, b, 2, &score) && score == 1.0);
    assert(total_variation_distance(a, c, 2, &score));
    assert(fabs(score - 0.75) < 1e-12);
    assert(total_variation_distance(c, a, 2, &score));
    assert(fabs(score - 0.75) < 1e-12);
    score = -9.0;
    assert(!total_variation_distance(a, b, 0, &score));
    assert(!total_variation_distance(NULL, b, 2, &score));
    assert(!total_variation_distance(a, b, 2, NULL));
    assert(!total_variation_distance(wrong, b, 2, &score));
    assert(!total_variation_distance(bad, b, 2, &score));
    assert(!total_variation_distance(nan, b, 2, &score));
    assert(score == -9.0);
    puts("C total variation tests passed");
    return 0;
}
