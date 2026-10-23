#include "scaled_dot_product_attention.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool scaled_dot_product_attention(
    const double *q, size_t d, const double *keys,
    const double *values, size_t m, size_t v, double *output) {
    if (!q || !keys || !values || !output || d==0 || m==0 || v==0 ||
        m>SIZE_MAX/d || m>SIZE_MAX/v ||
        m>SIZE_MAX/sizeof(double) || v>SIZE_MAX/sizeof(double)) return false;

    for (size_t i=0; i<d; ++i) if (!isfinite(q[i])) return false;
    for (size_t i=0; i<m*d; ++i) if (!isfinite(keys[i])) return false;
    for (size_t i=0; i<m*v; ++i) if (!isfinite(values[i])) return false;

    double *scores=malloc(m*sizeof(*scores));
    double *result=calloc(v,sizeof(*result));
    if (!scores || !result) {free(scores);free(result);return false;}
    const double scale=sqrt((double)d);
    double maximum=-INFINITY;
    for (size_t j=0; j<m; ++j) {
        double dot=0.0;
        for (size_t i=0; i<d; ++i) {
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
    for (size_t j=0; j<m; ++j) {
        const double delta=scores[j]-maximum;
        if (!isfinite(delta)) goto fail;
        scores[j]=exp(delta);
        if (!isfinite(scores[j])) goto fail;
        denominator+=scores[j];
        if (!isfinite(denominator)) goto fail;
    }
    if (denominator<=0.0) goto fail;
    for (size_t j=0; j<m; ++j) {
        const double probability=scores[j]/denominator;
        if (!isfinite(probability)) goto fail;
        for (size_t k=0; k<v; ++k) {
            const double term=probability*values[j*v+k];
            if (!isfinite(term)) goto fail;
            result[k]+=term;
            if (!isfinite(result[k])) goto fail;
        }
    }
    memcpy(output,result,v*sizeof(*result));
    free(scores);free(result);
    return true;
fail:
    free(scores);free(result);
    return false;
}
