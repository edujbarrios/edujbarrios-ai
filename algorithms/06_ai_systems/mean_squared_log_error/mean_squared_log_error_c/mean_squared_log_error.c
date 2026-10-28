#include "mean_squared_log_error.h"
#include <math.h>

bool mean_squared_log_error(const double *target, const double *prediction,
                            size_t n, double *out) {
    if (!target || !prediction || !out || n<1 || n>10000) return false;
    double sum=0.0;
    for (size_t i=0;i<n;++i) {
        const double t=target[i], p=prediction[i];
        if (!isfinite(t) || !isfinite(p) || t<0.0 || p<0.0 ||
            t>1000000.0 || p>1000000.0) return false;
        double difference=log1p(p)-log1p(t);
        sum+=difference*difference;
    }
    double result=sum/(double)n;
    if (!isfinite(result)) return false;
    *out=result;
    return true;
}
