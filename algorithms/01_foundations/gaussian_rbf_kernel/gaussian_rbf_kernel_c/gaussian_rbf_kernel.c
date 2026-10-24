#include "gaussian_rbf_kernel.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool gaussian_rbf_kernel(const double *points,size_t n,size_t d,
                         double sigma,double *kernel) {
    if (!points || !kernel || n==0 || n>128 || d==0 || d>32 ||
        !isfinite(sigma) || sigma<1e-6 || sigma>1e6)
        return false;
    for (size_t i=0;i<n*d;++i) {
        if (!isfinite(points[i]) || fabs(points[i])>1e6) return false;
    }
    double *scratch=malloc(n*n*sizeof(*scratch));
    if (!scratch) return false;
    for (size_t i=0;i<n;++i) {
        for (size_t j=i;j<n;++j) {
            double distance=0.0;
            for (size_t c=0;c<d;++c) {
                const double diff=points[i*d+c]-points[j*d+c];
                distance=hypot(distance,diff);
            }
            const double scaled=distance/sigma;
            const double value=exp(-0.5*scaled*scaled);
            if (!isfinite(distance) || !isfinite(scaled) ||
                !isfinite(value) || value<0 || value>1) {
                free(scratch);return false;
            }
            scratch[i*n+j]=value;
            scratch[j*n+i]=value;
        }
    }
    memcpy(kernel,scratch,n*n*sizeof(*scratch));
    free(scratch);
    return true;
}
