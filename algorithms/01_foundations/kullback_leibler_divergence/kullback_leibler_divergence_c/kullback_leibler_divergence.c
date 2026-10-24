#include "kullback_leibler_divergence.h"
#include <math.h>

bool kullback_leibler_divergence(const double *p,const double *q,
                                size_t n,double *result) {
    if (!p || !q || !result || n==0 || n>4096) return false;
    double sum_p=0.0,sum_q=0.0;
    bool missing_support=false;
    for (size_t i=0;i<n;++i) {
        const double a=p[i],b=q[i];
        if (!isfinite(a) || !isfinite(b) ||
            a<0.0 || a>1.0 || b<0.0 || b>1.0) return false;
        sum_p+=a;sum_q+=b;
        if (!isfinite(sum_p) || !isfinite(sum_q)) return false;
        if (a>0.0 && b==0.0) missing_support=true;
    }
    if (fabs(sum_p-1.0)>1e-9 || fabs(sum_q-1.0)>1e-9)
        return false;
    if (missing_support) {
        *result=INFINITY;
        return true;
    }
    double divergence=0.0;
    for (size_t i=0;i<n;++i) {
        if (p[i]==0.0) continue;
        const double term=p[i]*(log2(p[i])-log2(q[i]));
        if (!isfinite(term)) return false;
        divergence+=term;
        if (!isfinite(divergence)) return false;
    }
    if (divergence< -1e-12) return false;
    *result=fmax(0.0,divergence);
    return true;
}
