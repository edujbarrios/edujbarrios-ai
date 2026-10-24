#include "rmsprop_optimizer.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define DECAY 0.9
#define EPSILON 1e-8

bool rmsprop_optimizer(const double *parameters,const double *gradients,
    const double *previous_average,size_t n,double learning_rate,
    double *updated_parameters,double *updated_average) {
    if (!parameters || !gradients || !previous_average ||
        !updated_parameters || !updated_average || n==0 ||
        n>SIZE_MAX/sizeof(double) || !isfinite(learning_rate) ||
        learning_rate<=0) return false;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(parameters[i]) || !isfinite(gradients[i]) ||
            !isfinite(previous_average[i]) || previous_average[i]<0)
            return false;
    }
    double *next=malloc(n*sizeof(*next));
    double *avg=malloc(n*sizeof(*avg));
    if (!next || !avg) {free(next);free(avg);return false;}
    for (size_t i=0;i<n;++i) {
        const double g=gradients[i];
        const double square=g*g;
        if (!isfinite(square)) goto fail;
        avg[i]=DECAY*previous_average[i]+(1.0-DECAY)*square;
        if (!isfinite(avg[i]) || avg[i]<0) goto fail;
        const double denominator=sqrt(avg[i])+EPSILON;
        const double step=learning_rate*g/denominator;
        if (!isfinite(denominator) || !isfinite(step)) goto fail;
        next[i]=parameters[i]-step;
        if (!isfinite(next[i])) goto fail;
    }
    memcpy(updated_parameters,next,n*sizeof(*next));
    memcpy(updated_average,avg,n*sizeof(*avg));
    free(next);free(avg);
    return true;
fail:
    free(next);free(avg);
    return false;
}
