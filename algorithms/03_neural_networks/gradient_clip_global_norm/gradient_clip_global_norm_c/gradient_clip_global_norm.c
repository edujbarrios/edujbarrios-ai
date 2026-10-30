#include "gradient_clip_global_norm.h"
#include <math.h>

bool gradient_clip_global_norm(const double *gradients, size_t n, double max_norm, double *output) {
    if (!gradients || !output || n == 0 || !isfinite(max_norm) || max_norm <= 0.0) return false;

    double maximum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(gradients[i])) return false;
        double absolute = fabs(gradients[i]);
        if (absolute > maximum) maximum = absolute;
    }
    if (maximum == 0.0) {
        for (size_t i = 0; i < n; ++i) output[i] = gradients[i];
        return true;
    }
    double squares = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double scaled = gradients[i] / maximum;
        squares += scaled * scaled;
    }
    double factor = (max_norm / maximum) / sqrt(squares);
    if (factor > 1.0) factor = 1.0;
    for (size_t i = 0; i < n; ++i) output[i] = gradients[i] * factor;
    return true;
}
