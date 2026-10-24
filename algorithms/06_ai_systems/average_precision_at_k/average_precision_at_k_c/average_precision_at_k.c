#include "average_precision_at_k.h"
#include <math.h>
#include <stdint.h>

bool average_precision_at_k(const uint8_t *relevance,size_t n,size_t k,
                            size_t total_relevant,double *result) {
    if (!relevance || !result || n==0 || k==0 || k>n ||
        total_relevant>(size_t)INT32_MAX) return false;
    size_t observed=0;
    for (size_t i=0;i<n;++i) {
        if (relevance[i]>1) return false;
        observed+=(size_t)relevance[i];
    }
    if (observed>total_relevant) return false;
    if (total_relevant==0) {
        *result=0.0;
        return true;
    }
    size_t hits=0;
    double sum=0.0;
    for (size_t i=0;i<k;++i) {
        hits+=(size_t)relevance[i];
        if (relevance[i]) {
            sum+=(double)hits/(double)(i+1);
            if (!isfinite(sum)) return false;
        }
    }
    const size_t denominator=total_relevant<k?total_relevant:k;
    const double metric=sum/(double)denominator;
    if (!isfinite(metric) || metric<0.0 || metric>1.0+1e-12)
        return false;
    *result=metric>1.0?1.0:metric;
    return true;
}
