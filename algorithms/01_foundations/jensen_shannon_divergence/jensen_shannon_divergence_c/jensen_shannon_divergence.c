#include "jensen_shannon_divergence.h"
#include <math.h>

bool jensen_shannon_divergence(const double *p,const double *q,
                               size_t n,double *result) {
    if (!p || !q || !result || n==0 || n>4096) return false;
    double sum_p=0.0,sum_q=0.0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(p[i]) || !isfinite(q[i]) ||
            p[i]<0.0 || p[i]>1.0 || q[i]<0.0 || q[i]>1.0)
            return false;
        sum_p+=p[i];sum_q+=q[i];
    }
    if (!isfinite(sum_p) || !isfinite(sum_q) ||
        fabs(sum_p-1.0)>1e-9 || fabs(sum_q-1.0)>1e-9) return false;
    double js=0.0;
    for (size_t i=0;i<n;++i) {
        const double m=0.5*p[i]+0.5*q[i];
        if (p[i]>0.0) js+=0.5*p[i]*log2(p[i]/m);
        if (q[i]>0.0) js+=0.5*q[i]*log2(q[i]/m);
        if (!isfinite(js)) return false;
    }
    if (js<-1e-12 || js>1.0+1e-12) return false;
    if (js<0)js=0;
    if (js>1)js=1;
    *result=js;
    return true;
}
