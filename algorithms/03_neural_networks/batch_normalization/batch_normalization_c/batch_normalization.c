#include "batch_normalization.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define EPSILON 1e-5

bool batch_normalization(const double *x,size_t n,size_t d,
                         const double *gamma,const double *beta,
                         double *output){
    if (!x || !gamma || !beta || !output ||
        n==0 || n>512 || d==0 || d>32) return false;
    for (size_t j=0;j<d;++j) {
        if (!isfinite(gamma[j]) || !isfinite(beta[j]) ||
            fabs(gamma[j])>1e6 || fabs(beta[j])>1e6) return false;
    }
    for (size_t i=0;i<n*d;++i) {
        if (!isfinite(x[i]) || fabs(x[i])>1e6) return false;
    }
    double *scratch=malloc(n*d*sizeof(*scratch));
    if (!scratch) return false;
    double means[32]={0.0},variances[32]={0.0},scales[32];
    for (size_t j=0;j<d;++j) {
        for (size_t i=0;i<n;++i) means[j]+=x[i*d+j]/(double)n;
        for (size_t i=0;i<n;++i) {
            const double delta=x[i*d+j]-means[j];
            variances[j]+=(delta*delta)/(double)n;
        }
        const double divisor=sqrt(variances[j]+EPSILON);
        if (!isfinite(means[j]) || !isfinite(variances[j]) ||
            !isfinite(divisor) || divisor<=0.0) goto fail;
        scales[j]=gamma[j]/divisor;
        if (!isfinite(scales[j])) goto fail;
    }
    for (size_t i=0;i<n;++i) {
        for (size_t j=0;j<d;++j) {
            scratch[i*d+j]=(x[i*d+j]-means[j])*scales[j]+beta[j];
            if (!isfinite(scratch[i*d+j])) goto fail;
        }
    }
    memcpy(output,scratch,n*d*sizeof(*scratch));
    free(scratch);
    return true;
fail:
    free(scratch);
    return false;
}
