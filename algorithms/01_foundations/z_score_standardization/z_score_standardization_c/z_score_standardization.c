#include "z_score_standardization.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool z_score_standardization(const double *x,size_t n,size_t d,
    double *means,double *stds,double *normalized) {
    if (!x || !means || !stds || !normalized || n==0 || d==0 ||
        n>SIZE_MAX/d || d>SIZE_MAX/sizeof(double) ||
        n*d>SIZE_MAX/sizeof(double)) return false;
    const size_t cells=n*d;
    double *mu=calloc(d,sizeof(*mu));
    double *m2=calloc(d,sizeof(*m2));
    double *sd=malloc(d*sizeof(*sd));
    double *output=malloc(cells*sizeof(*output));
    if (!mu || !m2 || !sd || !output) goto fail;
    for (size_t i=0;i<n;++i) {
        for (size_t j=0;j<d;++j) {
            const double value=x[i*d+j];
            if (!isfinite(value)) goto fail;
            const double delta=value-mu[j];
            if (!isfinite(delta)) goto fail;
            mu[j]+=delta/(double)(i+1);
            if (!isfinite(mu[j])) goto fail;
            const double delta2=value-mu[j];
            const double term=delta*delta2;
            if (!isfinite(delta2) || !isfinite(term)) goto fail;
            m2[j]+=term;
            if (!isfinite(m2[j])) goto fail;
        }
    }
    for (size_t j=0;j<d;++j) {
        const double variance=m2[j]/(double)n;
        if (!isfinite(variance) || variance<0.0) goto fail;
        sd[j]=sqrt(variance);
        if (!isfinite(sd[j])) goto fail;
    }
    for (size_t i=0;i<n;++i) {
        for (size_t j=0;j<d;++j) {
            if (sd[j]==0.0) {
                output[i*d+j]=0.0;
            } else {
                const double delta=x[i*d+j]-mu[j];
                if (!isfinite(delta)) goto fail;
                output[i*d+j]=delta/sd[j];
                if (!isfinite(output[i*d+j])) goto fail;
            }
        }
    }
    memcpy(means,mu,d*sizeof(*mu));
    memcpy(stds,sd,d*sizeof(*sd));
    memcpy(normalized,output,cells*sizeof(*output));
    free(mu);free(m2);free(sd);free(output);
    return true;
fail:
    free(mu);free(m2);free(sd);free(output);
    return false;
}
