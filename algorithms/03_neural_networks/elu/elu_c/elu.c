#include "elu.h"
#include <math.h>

bool elu(const double *input, size_t n, double *output) {
    if (!input || !output || n==0 || n>10000) return false;
    for (size_t i=0;i<n;++i)
        if (!isfinite(input[i]) || fabs(input[i])>1000000.0)
            return false;
    for (size_t i=0;i<n;++i) {
        double x=input[i];
        output[i] = x>=0.0 ? x : expm1(x);
    }
    return true;
}
