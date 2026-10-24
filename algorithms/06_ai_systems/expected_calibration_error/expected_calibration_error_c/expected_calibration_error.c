#include "expected_calibration_error.h"
#include <math.h>
#include <stdlib.h>

bool expected_calibration_error(const double *probabilities,
    const uint8_t *labels,size_t n,size_t bins,double *result) {
    if (!probabilities || !labels || !result || n==0 ||
        bins==0 || bins>100) return false;
    size_t *counts=calloc(bins,sizeof(*counts));
    double *conf=calloc(bins,sizeof(*conf));
    double *correct=calloc(bins,sizeof(*correct));
    if (!counts || !conf || !correct) {
        free(counts);free(conf);free(correct);return false;
    }
    for (size_t i=0;i<n;++i) {
        const double p=probabilities[i];
        if (!isfinite(p) || p<0 || p>1 || labels[i]>1) goto fail;
        const unsigned predicted=(unsigned)(p>=0.5);
        const double confidence=p>=0.5?p:1.0-p;
        const double slot=floor(confidence*(double)bins);
        if (!isfinite(slot) || slot<0) goto fail;
        size_t b=(size_t)slot;
        if (b>=bins) b=bins-1;
        ++counts[b];
        conf[b]+=confidence;
        correct[b]+=(double)(predicted==labels[i]);
        if (!isfinite(conf[b]) || !isfinite(correct[b])) goto fail;
    }
    double total=0.0;
    for (size_t b=0;b<bins;++b) {
        if (counts[b]==0) continue;
        const double count=(double)counts[b];
        const double confidence=conf[b]/count;
        const double accuracy=correct[b]/count;
        total+=(count/(double)n)*fabs(accuracy-confidence);
        if (!isfinite(total)) goto fail;
    }
    if (total<0 || total>1.0+1e-12) goto fail;
    *result=total>1.0?1.0:total;
    free(counts);free(conf);free(correct);
    return true;
fail:
    free(counts);free(conf);free(correct);
    return false;
}
