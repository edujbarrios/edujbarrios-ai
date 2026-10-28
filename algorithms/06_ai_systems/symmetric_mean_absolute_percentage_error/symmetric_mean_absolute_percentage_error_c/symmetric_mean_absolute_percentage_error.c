#include "symmetric_mean_absolute_percentage_error.h"
#include <math.h>

bool symmetric_mean_absolute_percentage_error(
    const double *target, const double *prediction, size_t n, double *out) {
    if (!target || !prediction || !out || n==0 || n>10000) return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        const double t=target[i], p=prediction[i];
        if (!isfinite(t) || !isfinite(p) ||
            fabs(t)>1000000.0 || fabs(p)>1000000.0) return false;
        const double denom=fabs(t)+fabs(p);
        if (denom>0.0) total+=2.0*fabs(p-t)/denom;
    }
    const double mean=total/(double)n;
    if (!isfinite(mean)) return false;
    *out=fmax(0.0,fmin(2.0,mean));
    return true;
}
