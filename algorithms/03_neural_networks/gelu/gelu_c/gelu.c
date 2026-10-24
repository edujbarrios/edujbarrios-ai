#include "gelu.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define ALPHA 0.7978845608028654
#define BETA 0.044715

bool gelu(const double *x,size_t n,double *values,double *derivatives) {
    if (!x || !values || !derivatives || n==0 ||
        n>SIZE_MAX/sizeof(double)) return false;
    for (size_t i=0;i<n;++i) if (!isfinite(x[i])) return false;
    double *out=malloc(n*sizeof(*out));
    double *grad=malloc(n*sizeof(*grad));
    if (!out || !grad) {free(out);free(grad);return false;}
    for (size_t i=0;i<n;++i) {
        const double v=x[i];
        const double square=v*v;
        const double cubic=square*v;
        const double u=ALPHA*(v+BETA*cubic);
        if (!isfinite(square) || !isfinite(cubic) || !isfinite(u)) goto fail;
        const double t=tanh(u);
        const double gate=0.5*(1.0+t);
        const double factor=ALPHA*(1.0+3.0*BETA*square);
        const double slope=0.5*v*(1.0-t*t)*factor;
        out[i]=v*gate;
        grad[i]=gate+slope;
        if (!isfinite(factor) || !isfinite(slope) ||
            !isfinite(out[i]) || !isfinite(grad[i])) goto fail;
    }
    memcpy(values,out,n*sizeof(*out));
    memcpy(derivatives,grad,n*sizeof(*grad));
    free(out);free(grad);
    return true;
fail:
    free(out);free(grad);
    return false;
}
