#include "typical_sampling_filter.h"
#include <math.h>
bool typical_sampling_filter(const double *logits, size_t vocab,
                             double p, bool *keep) {
    if (!logits || !keep || vocab<1 || vocab>4096 ||
        !isfinite(p) || p<=0.0 || p>1.0) return false;
    double maximum=-INFINITY;
    for (size_t i=0;i<vocab;++i) {
        if (!isfinite(logits[i]) || fabs(logits[i])>1000.0) return false;
        if (logits[i]>maximum) maximum=logits[i];
    }
    double weights[4096], surprise[4096], deviations[4096], total=0.0;
    size_t order[4096];
    for (size_t i=0;i<vocab;++i) {
        weights[i]=exp(logits[i]-maximum);
        total+=weights[i];
        order[i]=i;
    }
    const double logz=log(total);
    double entropy=0.0;
    for (size_t i=0;i<vocab;++i) {
        surprise[i]=logz-(logits[i]-maximum);
        entropy+=(weights[i]/total)*surprise[i];
    }
    for (size_t i=0;i<vocab;++i)
        deviations[i]=fabs(surprise[i]-entropy);
    for (size_t i=0;i<vocab;++i) {
        size_t best=i;
        for (size_t j=i+1;j<vocab;++j) {
            const size_t a=order[j], b=order[best];
            if (deviations[a]<deviations[b] ||
                (deviations[a]==deviations[b] && a<b)) best=j;
        }
        const size_t temp=order[i];
        order[i]=order[best];
        order[best]=temp;
    }
    for (size_t i=0;i<vocab;++i) keep[i]=false;
    double mass=0.0;
    for (size_t k=0;k<vocab;++k) {
        const size_t i=order[k];
        keep[i]=true;
        mass+=weights[i]/total;
        if (p<1.0 && mass>=p) break;
    }
    return true;
}
