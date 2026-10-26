#include "huber_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1.5, 4}, b[] = {1, 2}, same[] = {1, 2};
    const double bad[] = {NAN}, high[] = {1e308}, low[] = {-1e308};
    const double zero[] = {0};
    double out = -9;
    assert(huber_loss(a, b, 2, 1, &out) && fabs(out - 0.8125) < 1e-12);
    assert(huber_loss(a, b, 2, 2, &out) && fabs(out - 1.0625) < 1e-12);
    assert(huber_loss(a, b, 2, 0.5, &out) && fabs(out - 0.5) < 1e-12);
    assert(huber_loss(same, same, 2, 1, &out) && out == 0);
    out = -9;
    assert(!huber_loss(a, b, 0, 1, &out));
    assert(!huber_loss(NULL, b, 2, 1, &out));
    assert(!huber_loss(a, b, 2, 0, &out));
    assert(!huber_loss(a, b, 2, INFINITY, &out));
    assert(!huber_loss(a, b, 2, 1, NULL));
    assert(!huber_loss(bad, zero, 1, 1, &out));
    assert(!huber_loss(high, low, 1, 1, &out));
    assert(!huber_loss(high, zero, 1, 2, &out));
    assert(out == -9);
    puts("C Huber loss tests passed");
    return 0;
}
