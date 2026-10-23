#include "precision_at_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint8_t judgments[] = {1, 0, 1, 1};
    const uint8_t invalid[] = {1, 0, 2};
    const uint8_t missing[] = {0, 0};
    double out = -1;
    assert(precision_at_k(judgments, 4, 3, &out));
    assert(fabs(out - 2.0/3.0) < 1e-12);
    assert(precision_at_k(judgments, 4, 4, &out) && out == 0.75);
    assert(precision_at_k(missing, 2, 2, &out) && out == 0.0);
    out = -1;
    assert(!precision_at_k(invalid, 3, 2, &out));
    assert(!precision_at_k(judgments, 4, 0, &out));
    assert(!precision_at_k(judgments, 4, 5, &out));
    assert(!precision_at_k(NULL, 0, 0, &out));
    assert(!precision_at_k(judgments, 4, 2, NULL));
    assert(out == -1);
    puts("C precision_at_k tests passed");
    return 0;
}
