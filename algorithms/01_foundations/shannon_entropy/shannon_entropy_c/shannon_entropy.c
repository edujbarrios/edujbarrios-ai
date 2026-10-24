#include "shannon_entropy.h"
#include <math.h>

bool shannon_entropy(const double *probabilities,size_t n,double *result) {
    if (!probabilities || !result || n==0 || n>4096) return false;
    double sum=0.0;
    for (size_t i=0;i<n;++i) {
        const double p=probabilities[i];
        if (!isfinite(p) || p<0.0 || p>1.0) return false;
        sum+=p;
        if (!isfinite(sum)) return false;
    }
    if (fabs(sum-1.0)>1e-9) return false;
    double information=0.0;
    for (size_t i=0;i<n;++i) {
        const double p=probabilities[i];
        if (p>0.0) {
            const double term=-p*log2(p);
            if (!isfinite(term)) return false;
            information+=term;
            if (!isfinite(information)) return false;
        }
    }
    if (information< -1e-12) return false;
    *result=fmax(0.0,information);
    return true;
}
