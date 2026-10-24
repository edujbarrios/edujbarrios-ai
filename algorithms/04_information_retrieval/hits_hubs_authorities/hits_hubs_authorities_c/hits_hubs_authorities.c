#include "hits_hubs_authorities.h"
#include <math.h>
#include <string.h>

bool hits_hubs_authorities(const double *weights,size_t n,
    double *authorities,double *hubs) {
    if (!weights || !authorities || !hubs || n==0 || n>64) return false;
    for (size_t i=0;i<n*n;++i)
        if (!isfinite(weights[i]) || weights[i]<0.0 || weights[i]>1.0)
            return false;
    double h[64],a[64],raw[64];
    const double initial=1.0/sqrt((double)n);
    for (size_t i=0;i<n;++i) {h[i]=initial;a[i]=0.0;}
    for (size_t step=0;step<100;++step) {
        for (size_t j=0;j<n;++j) {
            double total=0.0;
            for (size_t i=0;i<n;++i) total+=weights[i*n+j]*h[i];
            if (!isfinite(total)) return false;
            raw[j]=total;
        }
        double norm=0.0;
        for (size_t j=0;j<n;++j) norm=hypot(norm,raw[j]);
        if (!isfinite(norm)) return false;
        for (size_t j=0;j<n;++j) a[j]=norm>0?raw[j]/norm:0.0;
        for (size_t i=0;i<n;++i) {
            double total=0.0;
            for (size_t j=0;j<n;++j) total+=weights[i*n+j]*a[j];
            if (!isfinite(total)) return false;
            raw[i]=total;
        }
        norm=0.0;
        for (size_t i=0;i<n;++i) norm=hypot(norm,raw[i]);
        if (!isfinite(norm)) return false;
        for (size_t i=0;i<n;++i) {
            h[i]=norm>0?raw[i]/norm:0.0;
            if (!isfinite(h[i])) return false;
        }
    }
    memcpy(authorities,a,n*sizeof(double));
    memcpy(hubs,h,n*sizeof(double));
    return true;
}
