#include "random_hyperplane_hash.h"
#include <math.h>
#include <stdint.h>

bool random_hyperplane_hash(const double *x, size_t d,
                            const double *normals, size_t p,
                            uint32_t *signature) {
    if (!x || !normals || !signature || d == 0 || p == 0 || p > 30 ||
        p > SIZE_MAX / d) return false;
    bool nonzero_x = false;
    for (size_t i = 0; i < d; ++i) {
        if (!isfinite(x[i])) return false;
        if (x[i] != 0.0) nonzero_x = true;
    }
    if (!nonzero_x) return false;

    uint32_t result = 0;
    for (size_t j = 0; j < p; ++j) {
        bool nonzero_normal = false;
        double sum = 0.0;
        for (size_t i = 0; i < d; ++i) {
            const double normal = normals[j*d + i];
            if (!isfinite(normal)) return false;
            if (normal != 0.0) nonzero_normal = true;
            const double product = normal * x[i];
            if (!isfinite(product)) return false;
            sum += product;
            if (!isfinite(sum)) return false;
        }
        if (!nonzero_normal) return false;
        if (sum >= 0.0) result |= (uint32_t)1 << j;
    }
    *signature = result;
    return true;
}
