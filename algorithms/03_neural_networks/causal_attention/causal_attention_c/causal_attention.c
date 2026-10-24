#include "causal_attention.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool causal_attention(const double *queries, const double *keys,
                      const double *values, size_t t, size_t d,
                      size_t v, double *output) {
    if (!queries || !keys || !values || !output ||
        t==0 || d==0 || v==0 || t>SIZE_MAX/d || t>SIZE_MAX/v ||
        t>SIZE_MAX/sizeof(double) || t*v>SIZE_MAX/sizeof(double))
        return false;
    for (size_t i=0;i<t*d;++i)
        if (!isfinite(queries[i]) || !isfinite(keys[i])) return false;
    for (size_t i=0;i<t*v;++i)
        if (!isfinite(values[i])) return false;

    double *scores=malloc(t*sizeof(*scores));
    double *scratch=calloc(t*v,sizeof(*scratch));
    if (!scores || !scratch) {free(scores);free(scratch);return false;}
    const double scale=sqrt((double)d);
    for (size_t row=0;row<t;++row) {
        double maximum=-INFINITY;
        for (size_t j=0;j<=row;++j) {
            double dot=0.0;
            for (size_t i=0;i<d;++i) {
                const double term=queries[row*d+i]*keys[j*d+i];
                if (!isfinite(term)) goto fail;
                dot+=term;
                if (!isfinite(dot)) goto fail;
            }
            scores[j]=dot/scale;
            if (!isfinite(scores[j])) goto fail;
            if (scores[j]>maximum) maximum=scores[j];
        }
        double denominator=0.0;
        for (size_t j=0;j<=row;++j) {
            const double shift=scores[j]-maximum;
            if (!isfinite(shift)) goto fail;
            scores[j]=exp(shift);
            if (!isfinite(scores[j])) goto fail;
            denominator+=scores[j];
            if (!isfinite(denominator)) goto fail;
        }
        if (denominator<=0.0) goto fail;
        for (size_t j=0;j<=row;++j) {
            const double probability=scores[j]/denominator;
            for (size_t col=0;col<v;++col) {
                const double term=probability*values[j*v+col];
                if (!isfinite(term)) goto fail;
                scratch[row*v+col]+=term;
                if (!isfinite(scratch[row*v+col])) goto fail;
            }
        }
    }
    memcpy(output,scratch,t*v*sizeof(*scratch));
    free(scores);free(scratch);
    return true;
fail:
    free(scores);free(scratch);
    return false;
}
