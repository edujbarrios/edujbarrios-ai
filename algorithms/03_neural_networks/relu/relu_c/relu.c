#include "relu.h"
#include <math.h>

bool relu(const double *values, size_t n, double *output) {
    if (!output || (n > 0 && !values)) return false;
    for (size_t i=0; i<n; ++i) {
        if (!isfinite(values[i])) return false;
    }
    for (size_t i=0; i<n; ++i) {
        output[i] = values[i] > 0.0 ? values[i] : 0.0;
    }
    return true;
}
