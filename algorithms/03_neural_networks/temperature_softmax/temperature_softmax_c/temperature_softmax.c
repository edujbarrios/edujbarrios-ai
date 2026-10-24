#include "temperature_softmax.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool temperature_softmax(const double *logits,size_t n,double temperature,
                         double *probabilities) {
    if (!logits || !probabilities || n==0 || n>4096 ||
        !isfinite(temperature) || temperature<=0.0) return false;
    double maximum=-INFINITY;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(logits[i])) return false;
        if (logits[i]>maximum) maximum=logits[i];
    }
    double *weights=malloc(n*sizeof(*weights));
    if (!weights) return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        const double shifted=logits[i]-maximum;
        const double scaled=shifted/temperature;
        if (!isfinite(shifted) || !isfinite(scaled)) {
            free(weights);return false;
        }
        weights[i]=exp(scaled);
        total+=weights[i];
        if (!isfinite(weights[i]) || !isfinite(total)) {
            free(weights);return false;
        }
    }
    if (!(total>0.0)) {free(weights);return false;}
    for (size_t i=0;i<n;++i) {
        weights[i]/=total;
        if (!isfinite(weights[i])) {free(weights);return false;}
    }
    memcpy(probabilities,weights,n*sizeof(*weights));
    free(weights);
    return true;
}
