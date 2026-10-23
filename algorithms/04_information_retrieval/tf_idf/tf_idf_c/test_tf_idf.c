#include "tf_idf.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    double out = -1.0;
    assert(tf_idf(3, 100, 10, &out) && fabs(out - 3.0 * log(10.0)) < 1e-12);
    assert(tf_idf(0, 100, 10, &out) && out == 0.0);
    assert(tf_idf(12, 100, 100, &out) && out == 0.0);
    out = -1.0;
    assert(!tf_idf(1, 0, 0, &out));
    assert(!tf_idf(1, 100, 0, &out));
    assert(!tf_idf(1, 100, 101, &out));
    assert(!tf_idf(1, 100, 10, NULL));
    assert(out == -1.0);
    puts("C TF-IDF tests passed");
    return 0;
}
