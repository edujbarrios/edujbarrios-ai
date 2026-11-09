#include "root_mean_square.h"
#include <math.h>

bool root_mean_square(const double *values, size_t n, double *rms_out) {
    if (!values || !rms_out || n < 1 || n > 512) return false;
    double scale = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
        double a = fabs(values[i]);
        if (a > scale) scale = a;
    }
    if (scale == 0.0) {
        *rms_out = 0.0;
        return true;
    }
    double squares = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double v = values[i] / scale;
        squares += v * v;
    }
    const double result = scale * sqrt(squares / (double)n);
    if (!isfinite(result)) return false;
    *rms_out = result;
    return true;
}
