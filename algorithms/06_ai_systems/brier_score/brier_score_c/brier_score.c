#include "brier_score.h"
#include <math.h>
bool brier_score(const double *probabilities,const uint8_t *labels,
                 size_t n,double *result) {
    if (!probabilities || !labels || !result || n==0) return false;
    double sum=0.0;
    for (size_t i=0;i<n;++i) {
        const double p=probabilities[i];
        if (!isfinite(p) || p<0.0 || p>1.0 || labels[i]>1) return false;
        const double error=p-(double)labels[i];
        const double squared=error*error;
        sum+=squared;
        if (!isfinite(squared) || !isfinite(sum)) return false;
    }
    const double average=sum/(double)n;
    if (!isfinite(average) || average<0.0 || average>1.0+1e-12) return false;
    *result=average>1.0?1.0:average;
    return true;
}
