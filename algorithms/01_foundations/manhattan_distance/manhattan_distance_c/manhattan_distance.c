#include "manhattan_distance.h"
#include <math.h>
#include <stddef.h>

bool manhattan_distance(const double *left, const double *right,
                        size_t n, double *out) {
    if (out == NULL || (n != 0 && (left == NULL || right == NULL))) {
        return false;
    }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(left[i]) || !isfinite(right[i])) return false;
        double delta = left[i] - right[i];
        if (!isfinite(delta)) return false;
        total += fabs(delta);
        if (!isfinite(total)) return false;
    }
    *out = total;
    return true;
}
