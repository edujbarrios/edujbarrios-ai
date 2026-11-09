#include "interquartile_range.h"
#include <math.h>
#include <stdlib.h>

static int cmp(const void *a, const void *b) {
    const double x=*(const double *)a, y=*(const double *)b;
    return (x>y)-(x<y);
}
static double quantile(const double *a, size_t n, double p) {
    const double h=(double)(n-1)*p;
    const size_t i=(size_t)h;
    const double fraction=h-(double)i;
    if (fraction==0.0) return a[i];
    return (1.0-fraction)*a[i]+fraction*a[i+1];
}
bool interquartile_range(const double *values,size_t n,double *iqr_out) {
    if (!values || !iqr_out || n<1 || n>512) return false;
    double sorted[512];
    for (size_t i=0;i<n;++i) {
        if (!isfinite(values[i])) return false;
        sorted[i]=values[i];
    }
    qsort(sorted,n,sizeof(sorted[0]),cmp);
    double result=quantile(sorted,n,0.75)-quantile(sorted,n,0.25);
    if (!isfinite(result)) return false;
    *iqr_out=result<0.0?0.0:result;
    return true;
}
