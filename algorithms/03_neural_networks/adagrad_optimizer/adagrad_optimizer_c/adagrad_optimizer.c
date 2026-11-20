#include "adagrad_optimizer.h"
#include <math.h>

bool adagrad_optimizer(const double *parameters, const double *gradients,
                       const double *accumulator, size_t n,
                       double learning_rate, double epsilon,
                       double *next_parameters, double *next_accumulator) {
    if (!parameters || !gradients || !accumulator ||
        !next_parameters || !next_accumulator ||
        !isfinite(learning_rate) || learning_rate <= 0 ||
        !isfinite(epsilon) || epsilon <= 0) return false;

    /* Check all computations before writing either output. */
    for (size_t i = 0; i < n; ++i) {
        const double w = parameters[i], g = gradients[i], h = accumulator[i];
        if (!isfinite(w) || !isfinite(g) || !isfinite(h) || h < 0) return false;
        const double updated_h = h + g * g;
        const double denominator = sqrt(updated_h) + epsilon;
        const double step = learning_rate * g / denominator;
        const double updated_w = w - step;
        if (!isfinite(updated_h) || !isfinite(denominator) ||
            !isfinite(step) || !isfinite(updated_w)) return false;
    }
    for (size_t i = 0; i < n; ++i) {
        const double updated_h = accumulator[i] + gradients[i] * gradients[i];
        next_accumulator[i] = updated_h;
        next_parameters[i] = parameters[i] -
            learning_rate * gradients[i] / (sqrt(updated_h) + epsilon);
    }
    return true;
}
