#include "gradient_clip_global_norm.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double g[] = {3, 4};
    const double zero[] = {0, 0};
    const double extreme[] = {1e308, -1e308};
    const double invalid[] = {1, NAN};
    double output[] = {-1, -1};
    assert(gradient_clip_global_norm(g, 2, 1, output));
    assert(fabs(output[0] - 0.6) < 1e-12 && fabs(output[1] - 0.8) < 1e-12);
    assert(gradient_clip_global_norm(g, 2, 5, output) && output[0] == 3 && output[1] == 4);
    assert(gradient_clip_global_norm(zero, 2, 1, output) && output[0] == 0 && output[1] == 0);
    assert(gradient_clip_global_norm(extreme, 2, 1, output));
    assert(isfinite(output[0]) && fabs(output[0] - sqrt(0.5)) < 1e-12);
    assert(fabs(output[1] + sqrt(0.5)) < 1e-12);
    double inplace[] = {3, 4};
    assert(gradient_clip_global_norm(inplace, 2, 1, inplace));
    assert(fabs(inplace[0] - 0.6) < 1e-12);
    output[0] = -1;
    assert(!gradient_clip_global_norm(invalid, 2, 1, output) && output[0] == -1);
    assert(!gradient_clip_global_norm(g, 2, 0, output) && output[0] == -1);
    assert(!gradient_clip_global_norm(g, 2, NAN, output));
    assert(!gradient_clip_global_norm(g, 0, 1, output));
    assert(!gradient_clip_global_norm(NULL, 2, 1, output));
    assert(!gradient_clip_global_norm(g, 2, 1, NULL));
    puts("C gradient norm clipping tests passed");
    return 0;
}
