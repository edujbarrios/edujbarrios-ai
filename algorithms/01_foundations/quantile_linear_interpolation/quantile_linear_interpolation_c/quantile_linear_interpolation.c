#include "quantile_linear_interpolation.h"
#include <math.h>
#include <stdlib.h>

static int compare_double(const void *a, const void *b) {
    const double x=*(const double *)a, y=*(const double *)b;
    return (x>y)-(x<y);
}
bool quantile_linear_interpolation(
    const double *values, size_t n, double probability, double *quantile_out) {
    if (!values || !quantile_out || n<1 || n>512 ||
        !isfinite(probability) || probability<0.0 || probability>1.0)
        return false;
    double sorted[512];
    for (size_t i=0; i<n; ++i) {
        if (!isfinite(values[i])) return false;
        sorted[i]=values[i];
    }
    qsort(sorted,n,sizeof(sorted[0]),compare_double);
    const double h=((double)n-1.0)*probability;
    const size_t i=(size_t)floor(h);
    const double f=h-(double)i;
    const double result=f==0.0?sorted[i]:(1.0-f)*sorted[i]+f*sorted[i+1];
    if (!isfinite(result)) return false;
    *quantile_out=result;
    return true;
}
