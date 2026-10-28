#include "entropy_impurity.h"
#include <math.h>

bool entropy_impurity(const uint32_t *counts, size_t n, double *out) {
    if (!counts || !out || n < 1 || n > 128) return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        if (counts[i]>1000000U) return false;
        total+=(double)counts[i];
    }
    if (total==0.0) return false;
    double result=0.0;
    for (size_t i=0;i<n;++i) {
        if (counts[i]!=0) {
            double p=(double)counts[i]/total;
            result-=p*log2(p);
        }
    }
    if (!isfinite(result)) return false;
    *out=fmax(0.0,result);
    return true;
}
