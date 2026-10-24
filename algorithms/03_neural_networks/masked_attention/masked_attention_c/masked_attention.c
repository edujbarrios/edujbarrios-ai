#include "masked_attention.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool masked_attention(const double *q, size_t d, const double *keys,
                      const double *values, const uint8_t *mask,
                      size_t m, size_t v, double *out) {
    if (!q || !keys || !values || !mask || !out || d==0 || m==0 || v==0 ||
        m>SIZE_MAX/d || m>SIZE_MAX/v ||
        m>SIZE_MAX/sizeof(double) || v>SIZE_MAX/sizeof(double))
        return false;

    bool any_allowed=false;
    for (size_t i=0;i<d;++i) if (!isfinite(q[i])) return false;
    for (size_t i=0;i<m*d;++i) if (!isfinite(keys[i])) return false;
    for (size_t i=0;i<m*v;++i) if (!isfinite(values[i])) return false;
    for (size_t j=0;j<m;++j) {
        if (mask[j]>1) return false;
        if (mask[j]==1) any_allowed=true;
    }
    if (!any_allowed) return false;

    double *scores=malloc(m*sizeof(*scores));
    double *result=calloc(v,sizeof(*result));
    if (!scores || !result) {free(scores);free(result);return false;}

    const double scale=sqrt((double)d);
    double maximum=-INFINITY;
    for (size_t j=0;j<m;++j) {
        scores[j]=0.0;
        if (!mask[j]) continue;
        double dot=0.0;
        for (size_t i=0;i<d;++i) {
            const double product=q[i]*keys[j*d+i];
            if (!isfinite(product)) goto fail;
            dot+=product;
            if (!isfinite(dot)) goto fail;
        }
        scores[j]=dot/scale;
        if (!isfinite(scores[j])) goto fail;
        if (scores[j]>maximum) maximum=scores[j];
    }

    double denominator=0.0;
    for (size_t j=0;j<m;++j) {
        if (!mask[j]) continue;
        const double shifted=scores[j]-maximum;
        if (!isfinite(shifted)) goto fail;
        scores[j]=exp(shifted);
        if (!isfinite(scores[j])) goto fail;
        denominator+=scores[j];
        if (!isfinite(denominator)) goto fail;
    }
    if (denominator<=0.0) goto fail;

    for (size_t j=0;j<m;++j) {
        if (!mask[j]) continue;
        const double weight=scores[j]/denominator;
        for (size_t col=0;col<v;++col) {
            const double term=weight*values[j*v+col];
            if (!isfinite(term)) goto fail;
            result[col]+=term;
            if (!isfinite(result[col])) goto fail;
        }
    }
    memcpy(out,result,v*sizeof(*result));
    free(scores);free(result);
    return true;
fail:
    free(scores);free(result);
    return false;
}
