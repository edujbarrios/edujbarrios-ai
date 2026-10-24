#include "ridge_regression_1d.h"
#include <math.h>

bool ridge_regression_1d(const double *x,const double *y,size_t n,
                         double lambda,double *intercept,double *slope) {
    if (!x || !y || !intercept || !slope || n==0 || n>4096 ||
        !isfinite(lambda) || lambda<0.0 || lambda>1e12)
        return false;
    double mx=0.0,my=0.0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(x[i]) || !isfinite(y[i]) ||
            fabs(x[i])>1e6 || fabs(y[i])>1e6) return false;
        mx+=x[i]/(double)n;
        my+=y[i]/(double)n;
    }
    if (!isfinite(mx) || !isfinite(my)) return false;
    double sxx=0.0,sxy=0.0;
    for (size_t i=0;i<n;++i) {
        const double dx=x[i]-mx,dy=y[i]-my;
        sxx+=dx*dx;
        sxy+=dx*dy;
        if (!isfinite(sxx) || !isfinite(sxy)) return false;
    }
    const double denom=sxx+lambda;
    if (!isfinite(denom)) return false;
    const double fitted_slope=denom==0.0?0.0:sxy/denom;
    const double fitted_intercept=my-fitted_slope*mx;
    if (!isfinite(fitted_slope) || !isfinite(fitted_intercept))
        return false;
    *intercept=fitted_intercept;
    *slope=fitted_slope;
    return true;
}
