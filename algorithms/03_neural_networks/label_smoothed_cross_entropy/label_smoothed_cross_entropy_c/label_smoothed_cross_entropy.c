#include "label_smoothed_cross_entropy.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool label_smoothed_cross_entropy(const double *logits,size_t n,
    size_t target,double epsilon,double *loss,double *gradients) {
    if (!logits || !loss || !gradients || n==0 || n>4096 ||
        target>=n || !isfinite(epsilon) || epsilon<0 || epsilon>1)
        return false;
    double maximum=-INFINITY;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(logits[i])) return false;
        if (logits[i]>maximum) maximum=logits[i];
    }
    double *scratch=malloc(n*sizeof(*scratch));
    if (!scratch) return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        const double shifted=logits[i]-maximum;
        if (!isfinite(shifted)) goto fail;
        scratch[i]=shifted;
        const double weight=exp(shifted);
        total+=weight;
        if (!isfinite(weight) || !isfinite(total)) goto fail;
    }
    if (!(total>0)) goto fail;
    const double logz=log(total);
    if (!isfinite(logz)) goto fail;
    double accumulated=0.0;
    const double uniform=epsilon/(double)n;
    for (size_t i=0;i<n;++i) {
        const double q=uniform+(i==target ? 1.0-epsilon : 0.0);
        const double logloss=logz-scratch[i];
        const double term=q*logloss;
        scratch[i]=exp(scratch[i])/total-q;
        accumulated+=term;
        if (!isfinite(logloss) || !isfinite(term) ||
            !isfinite(accumulated) || !isfinite(scratch[i])) goto fail;
    }
    if (accumulated< -1e-12) goto fail;
    memcpy(gradients,scratch,n*sizeof(*scratch));
    *loss=accumulated<0?0.0:accumulated;
    free(scratch);
    return true;
fail:
    free(scratch);
    return false;
}
