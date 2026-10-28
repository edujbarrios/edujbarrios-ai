#include "root_mean_squared_error.h"
#include <math.h>

bool root_mean_squared_error(const double *target, const double *prediction,
                             size_t n, double *out) {
    if (!target || !prediction || !out || n==0 || n>10000) return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        double t=target[i],p=prediction[i];
        if (!isfinite(t) || !isfinite(p) ||
            fabs(t)>1000000.0 || fabs(p)>1000000.0) return false;
        double d=p-t;
        total+=d*d;
    }
    double result=sqrt(total/(double)n);
    if (!isfinite(result)) return false;
    *out=result;
    return true;
}
