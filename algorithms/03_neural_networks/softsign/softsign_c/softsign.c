#include "softsign.h"
#include <math.h>

bool softsign(const double *inputs, size_t n, double *output) {
    if (!inputs || !output || n == 0 || n > 10000) return false;
    for (size_t i=0; i<n; ++i)
        if (!isfinite(inputs[i]) || fabs(inputs[i]) > 1000000.0)
            return false;
    for (size_t i=0; i<n; ++i)
        output[i] = inputs[i]/(1.0+fabs(inputs[i]));
    return true;
}
