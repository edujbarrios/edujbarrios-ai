#include "layer_normalization.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool layer_normalization(const double *input, const double *gamma,
                         const double *beta, size_t n, double epsilon,
                         double *output) {
    if (!input || !gamma || !beta || !output || n==0 ||
        n>SIZE_MAX/sizeof(double) || !isfinite(epsilon) || epsilon<=0.0)
        return false;
    double mean=0.0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(input[i]) || !isfinite(gamma[i]) ||
            !isfinite(beta[i])) return false;
        const double delta=input[i]-mean;
        if (!isfinite(delta)) return false;
        mean+=delta/(double)(i+1);
        if (!isfinite(mean)) return false;
    }
    double squared_sum=0.0;
    for (size_t i=0;i<n;++i) {
        const double delta=input[i]-mean;
        const double square=delta*delta;
        if (!isfinite(delta) || !isfinite(square)) return false;
        squared_sum+=square;
        if (!isfinite(squared_sum)) return false;
    }
    const double variance=squared_sum/(double)n;
    const double divisor=sqrt(variance+epsilon);
    if (!isfinite(variance) || !isfinite(divisor) || divisor<=0.0)
        return false;
    double *scratch=malloc(n*sizeof(*scratch));
    if (!scratch) return false;
    for (size_t i=0;i<n;++i) {
        const double centered=input[i]-mean;
        const double normalized=centered/divisor;
        const double weighted=gamma[i]*normalized;
        scratch[i]=weighted+beta[i];
        if (!isfinite(centered) || !isfinite(normalized) ||
            !isfinite(weighted) || !isfinite(scratch[i])) {
            free(scratch);
            return false;
        }
    }
    memcpy(output,scratch,n*sizeof(*scratch));
    free(scratch);
    return true;
}
