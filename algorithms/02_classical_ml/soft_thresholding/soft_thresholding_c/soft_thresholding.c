#include "soft_thresholding.h"
#include <math.h>
bool soft_thresholding(const double *values, size_t n, double threshold, double *output) {
    if (!values || !output || n == 0 || !isfinite(threshold) || threshold < 0.0) return false;
    for (size_t i = 0; i < n; ++i) if (!isfinite(values[i])) return false;
    for (size_t i = 0; i < n; ++i) {
        double v = values[i];
        output[i] = v > threshold ? v - threshold :
                    v < -threshold ? v + threshold : 0.0;
    }
    return true;
}
