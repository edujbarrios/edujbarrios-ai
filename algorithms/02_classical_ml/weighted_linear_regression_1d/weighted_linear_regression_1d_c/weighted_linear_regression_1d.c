#include "weighted_linear_regression_1d.h"
#include <math.h>

bool weighted_linear_regression_1d(const double *x, const double *y,
    const double *weights, size_t n, double *slope, double *intercept) {
    if (!x || !y || !weights || !slope || !intercept || slope==intercept ||
        n<2 || n>128) return false;
    double weight_total=0.0, sx=0.0, sy=0.0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(x[i]) || !isfinite(y[i]) || !isfinite(weights[i]) ||
            fabs(x[i])>1000000.0 || fabs(y[i])>1000000.0 ||
            weights[i]<0.0 || weights[i]>1000000.0) return false;
        weight_total+=weights[i];
        sx+=weights[i]*x[i];
        sy+=weights[i]*y[i];
    }
    if (weight_total==0.0) return false;
    double mx=sx/weight_total, my=sy/weight_total;
    double numer=0.0, denom=0.0;
    for (size_t i=0;i<n;++i) {
        double dx=x[i]-mx;
        numer+=weights[i]*dx*(y[i]-my);
        denom+=weights[i]*dx*dx;
    }
    if (!isfinite(denom) || denom<=0.0) return false;
    double b=numer/denom, a=my-b*mx;
    if (!isfinite(a) || !isfinite(b)) return false;
    *slope=b;
    *intercept=a;
    return true;
}
