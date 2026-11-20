#include "adagrad_optimizer.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double w[] = {1, -1}, g[] = {2, -4}, h[] = {0, 0};
    const double bad_h[] = {0, -1}, huge[] = {1e308, 0};
    const double nan_w[] = {NAN, -1};
    double out_w[] = {-99, -99}, out_h[] = {-99, -99};
    assert(adagrad_optimizer(w, g, h, 2, .1, 1, out_w, out_h));
    assert(out_h[0] == 4 && out_h[1] == 16);
    assert(fabs(out_w[0] - (1.0 - .2/3.0)) < 1e-12);
    assert(fabs(out_w[1] - (-1.0 + .4/5.0)) < 1e-12);
    assert(adagrad_optimizer(w, g, h, 0, .1, 1, out_w, out_h));
    out_w[0] = out_h[0] = -99;
    assert(!adagrad_optimizer(w, g, bad_h, 2, .1, 1, out_w, out_h));
    assert(!adagrad_optimizer(w, huge, h, 2, .1, 1, out_w, out_h));
    assert(!adagrad_optimizer(nan_w, g, h, 2, .1, 1, out_w, out_h));
    assert(!adagrad_optimizer(w, g, h, 2, 0, 1, out_w, out_h));
    assert(!adagrad_optimizer(w, g, h, 2, .1, 0, out_w, out_h));
    assert(!adagrad_optimizer(w, g, h, 2, .1, 1, NULL, out_h));
    assert(out_w[0] == -99 && out_h[0] == -99);
    puts("C AdaGrad tests passed");
    return 0;
}
