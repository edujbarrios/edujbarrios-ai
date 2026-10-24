#include "swish.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool swish(const double *x, size_t n, double *values, double *derivatives) {
    if (!x || !values || !derivatives || n == 0 ||
        n > SIZE_MAX/sizeof(double)) return false;
    for (size_t i = 0; i < n; ++i)
        if (!isfinite(x[i])) return false;
    double *v=malloc(n*sizeof(*v)), *d=malloc(n*sizeof(*d));
    if (!v || !d) {free(v);free(d);return false;}
    for (size_t i = 0; i < n; ++i) {
        const double a=x[i];
        double s;
        if (a>=0.0) {
            const double z=exp(-a);
            s=1.0/(1.0+z);
        } else {
            const double z=exp(a);
            s=z/(1.0+z);
        }
        v[i]=a*s;
        d[i]=s+(a*s)*(1.0-s);
        if (!isfinite(v[i]) || !isfinite(d[i])) {
            free(v);free(d);return false;
        }
    }
    memcpy(values,v,n*sizeof(*v));
    memcpy(derivatives,d,n*sizeof(*d));
    free(v);free(d);
    return true;
}
