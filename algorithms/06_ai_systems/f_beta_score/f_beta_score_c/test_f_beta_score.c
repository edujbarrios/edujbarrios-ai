#include "f_beta_score.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    double score = -9;
    assert(f_beta_score(8, 2, 4, 1, &score));
    assert(fabs(score - 8.0 / 11.0) < 1e-12);
    assert(f_beta_score(8, 2, 4, 2, &score));
    assert(fabs(score - 20.0 / 29.0) < 1e-12);
    assert(f_beta_score(8, 2, 4, 0.5, &score));
    assert(fabs(score - 10.0 / 13.0) < 1e-12);
    assert(f_beta_score(8, 2, 4, 1e-300, &score));
    assert(fabs(score - 0.8) < 1e-12);
    assert(f_beta_score(8, 2, 4, 1e300, &score));
    assert(fabs(score - 2.0 / 3.0) < 1e-12);
    assert(f_beta_score(0, 0, 0, 1, &score) && score == 0);
    assert(f_beta_score(INT32_MAX, 0, 0, 1, &score) && score == 1);
    score = -9;
    assert(!f_beta_score(-1, 0, 0, 1, &score));
    assert(!f_beta_score(1, 0, 0, 0, &score));
    assert(!f_beta_score(1, 0, 0, INFINITY, &score));
    assert(!f_beta_score(1, 0, 0, NAN, &score));
    assert(!f_beta_score(1, 0, 0, 1, NULL));
    assert(score == -9);
    puts("C F-beta tests passed");
    return 0;
}
