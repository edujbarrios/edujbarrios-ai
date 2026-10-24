#include "binary_focal_loss.h"
#include <math.h>

bool binary_focal_loss(const double *probabilities,const uint8_t *labels,
                       size_t n,double *result) {
    if (!probabilities || !labels || !result || n==0) return false;
    double sum=0.0;
    for (size_t i=0;i<n;++i) {
        const double p=probabilities[i];
        const uint8_t label=labels[i];
        if (!isfinite(p) || p<0.0 || p>1.0 || label>1) return false;
        const double pt=label==1?p:1.0-p;
        const double q=fmax(pt,1e-12);
        const double alpha=label==1?0.25:0.75;
        const double difficulty=1.0-q;
        const double sample=-alpha*difficulty*difficulty*log(q);
        if (!isfinite(sample) || sample<0.0) return false;
        sum+=sample;
        if (!isfinite(sum)) return false;
    }
    const double mean=sum/(double)n;
    if (!isfinite(mean) || mean<0.0) return false;
    *result=mean;
    return true;
}
