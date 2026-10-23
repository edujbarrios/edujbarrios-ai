#include "logistic_regression.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {0.0, 1.0, 2.0, 3.0};
    const double y[] = {0.0, 0.0, 1.0, 1.0};
    const double bad_label[] = {0.0, 2.0};
    const double nonfinite[] = {NAN, 1.0};
    const double extreme[] = {1e308, 0.0};
    const double binary[] = {0.0, 1.0};
    double weight = -99.0, bias = -99.0;
    assert(logistic_regression(x, y, 4, 0.2, 2000, &weight, &bias));
    assert(weight > 0.0 && bias < 0.0);
    assert(weight * 0.0 + bias < -2.0);
    assert(weight * 3.0 + bias > 2.0);
    assert(logistic_regression(x, y, 4, 0.2, 1, &weight, &bias));
    assert(isfinite(weight) && isfinite(bias));

    weight = -99.0; bias = -99.0;
    assert(!logistic_regression(x, y, 0, 0.2, 20, &weight, &bias));
    assert(!logistic_regression(x, y, 4, 0.0, 20, &weight, &bias));
    assert(!logistic_regression(x, y, 4, 0.2, 0, &weight, &bias));
    assert(!logistic_regression(x, y, 4, INFINITY, 20, &weight, &bias));
    assert(!logistic_regression(x, bad_label, 2, 0.2, 20, &weight, &bias));
    assert(!logistic_regression(nonfinite, binary, 2, 0.2, 20, &weight, &bias));
    assert(!logistic_regression(extreme, binary, 2, 1e308, 20, &weight, &bias));
    assert(!logistic_regression(x, y, 4, 0.2, 20, &weight, &weight));
    assert(weight == -99.0 && bias == -99.0);
    puts("C logistic regression tests passed");
    return 0;
}
