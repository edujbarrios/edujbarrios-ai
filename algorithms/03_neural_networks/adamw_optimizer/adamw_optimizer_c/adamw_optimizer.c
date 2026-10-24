#include "adamw_optimizer.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define BETA1 0.9
#define BETA2 0.999
#define EPS 1e-8
#define DECAY 0.01

bool adamw_optimizer(const double *theta,const double *gradient,
    const double *m,const double *v,size_t n,size_t step,double learning_rate,
    double *theta_next,double *m_next,double *v_next) {
    if (!theta || !gradient || !m || !v || !theta_next || !m_next || !v_next ||
        n==0 || n>SIZE_MAX/sizeof(double) || step==0 || step>1000000 ||
        !isfinite(learning_rate) || learning_rate<=0) return false;
    const double correction1=1.0-pow(BETA1,(double)step);
    const double correction2=1.0-pow(BETA2,(double)step);
    if (!(correction1>0) || !(correction2>0) ||
        !isfinite(correction1) || !isfinite(correction2)) return false;
    for (size_t i=0;i<n;++i)
        if (!isfinite(theta[i]) || !isfinite(gradient[i]) ||
            !isfinite(m[i]) || !isfinite(v[i]) || v[i]<0) return false;
    double *parameters=malloc(n*sizeof(double));
    double *first=malloc(n*sizeof(double));
    double *second=malloc(n*sizeof(double));
    if (!parameters || !first || !second) {
        free(parameters);free(first);free(second);return false;
    }
    for (size_t i=0;i<n;++i) {
        const double square=gradient[i]*gradient[i];
        first[i]=BETA1*m[i]+(1.0-BETA1)*gradient[i];
        second[i]=BETA2*v[i]+(1.0-BETA2)*square;
        if (!isfinite(square) || !isfinite(first[i]) ||
            !isfinite(second[i]) || second[i]<0) goto fail;
        const double corrected_first=first[i]/correction1;
        const double corrected_second=second[i]/correction2;
        const double denominator=sqrt(corrected_second)+EPS;
        const double adaptive=corrected_first/denominator;
        const double shrinkage=DECAY*theta[i];
        const double update=learning_rate*(adaptive+shrinkage);
        parameters[i]=theta[i]-update;
        if (!isfinite(corrected_first) || !isfinite(corrected_second) ||
            !isfinite(denominator) || !(denominator>0) ||
            !isfinite(adaptive) || !isfinite(shrinkage) ||
            !isfinite(update) || !isfinite(parameters[i])) goto fail;
    }
    memcpy(theta_next,parameters,n*sizeof(double));
    memcpy(m_next,first,n*sizeof(double));
    memcpy(v_next,second,n*sizeof(double));
    free(parameters);free(first);free(second);return true;
fail:
    free(parameters);free(first);free(second);return false;
}
