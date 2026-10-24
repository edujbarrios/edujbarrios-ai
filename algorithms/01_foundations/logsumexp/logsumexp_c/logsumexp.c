#include "logsumexp.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool logsumexp(const double *x,size_t n,double *value,double *gradient) {
    if (!x || !value || !gradient || n==0 || n>4096) return false;
    double maximum=-INFINITY;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(x[i])) return false;
        if (x[i]>maximum) maximum=x[i];
    }
    double *weights=malloc(n*sizeof(*weights));
    if (!weights) return false;
    double normalizer=0.0;
    for (size_t i=0;i<n;++i) {
        const double shifted=x[i]-maximum;
        if (!isfinite(shifted)) {free(weights);return false;}
        weights[i]=exp(shifted);
        normalizer+=weights[i];
        if (!isfinite(weights[i]) || !isfinite(normalizer)) {
            free(weights);return false;
        }
    }
    if (normalizer<=0.0) {free(weights);return false;}
    const double result=maximum+log(normalizer);
    if (!isfinite(result)) {free(weights);return false;}
    for (size_t i=0;i<n;++i) {
        weights[i]/=normalizer;
        if (!isfinite(weights[i])) {free(weights);return false;}
    }
    memcpy(gradient,weights,n*sizeof(*weights));
    *value=result;
    free(weights);
    return true;
}
