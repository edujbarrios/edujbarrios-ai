#include "dense_forward.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool dense_forward(const double *input, size_t input_dim,
                   const double *weights, const double *bias, size_t output_dim,
                   double *output) {
    if (!input || !weights || !bias || !output ||
        input_dim == 0 || output_dim == 0 ||
        output_dim > SIZE_MAX / input_dim ||
        output_dim > SIZE_MAX / sizeof(double)) return false;

    for (size_t i = 0; i < input_dim; ++i) {
        if (!isfinite(input[i])) return false;
    }
    for (size_t i = 0; i < output_dim * input_dim; ++i) {
        if (!isfinite(weights[i])) return false;
    }
    for (size_t i = 0; i < output_dim; ++i) {
        if (!isfinite(bias[i])) return false;
    }

    double *scratch = malloc(output_dim * sizeof(*scratch));
    if (!scratch) return false;
    for (size_t j = 0; j < output_dim; ++j) {
        double sum = 0.0;
        for (size_t i = 0; i < input_dim; ++i) {
            const double term = weights[j * input_dim + i] * input[i];
            if (!isfinite(term)) { free(scratch); return false; }
            sum += term;
            if (!isfinite(sum)) { free(scratch); return false; }
        }
        const double z = sum + bias[j];
        if (!isfinite(z)) { free(scratch); return false; }
        scratch[j] = z > 0.0 ? z : 0.0;
    }
    memcpy(output, scratch, output_dim * sizeof(*output));
    free(scratch);
    return true;
}
