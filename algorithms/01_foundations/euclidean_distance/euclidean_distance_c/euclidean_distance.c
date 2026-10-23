#include "euclidean_distance.h"

#include <math.h>
#include <stddef.h>

bool euclidean_distance(const double *left, const double *right,
                        size_t n, double *out) {
    if (out == NULL || (n > 0 && (left == NULL || right == NULL))) {
        return false;
    }

    double distance = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(left[i]) || !isfinite(right[i])) {
            return false;
        }
        const double difference = left[i] - right[i];
        if (!isfinite(difference)) {
            return false;
        }
        distance = hypot(distance, difference);
        if (!isfinite(distance)) {
            return false;
        }
    }
    *out = distance;
    return true;
}
