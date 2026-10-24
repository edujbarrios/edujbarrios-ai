#include "huber_loss.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool huber_loss(const double *predictions,const double *targets,size_t n,
                double delta,double *mean_loss,double *gradients) {
    if (!predictions || !targets || !mean_loss || !gradients ||
        n==0 || n>SIZE_MAX/sizeof(double) ||
        !isfinite(delta) || delta<=0.0) return false;
    double *scratch=malloc(n*sizeof(*scratch));
    if (!scratch) return false;
    double sum=0.0;
    for (size_t i=0;i<n;++i) {
        const double prediction=predictions[i],target=targets[i];
        if (!isfinite(prediction) || !isfinite(target)) goto fail;
        const double error=prediction-target;
        if (!isfinite(error)) goto fail;
        const double magnitude=fabs(error);
        double sample;
        double gradient;
        if (magnitude<=delta) {
            sample=0.5*error*error;
            gradient=error/(double)n;
        } else {
            sample=delta*(magnitude-0.5*delta);
            gradient=(error>0?delta:-delta)/(double)n;
        }
        if (!isfinite(sample) || !isfinite(gradient)) goto fail;
        sum+=sample;
        if (!isfinite(sum)) goto fail;
        scratch[i]=gradient;
    }
    const double result=sum/(double)n;
    if (!isfinite(result) || result<0) goto fail;
    memcpy(gradients,scratch,n*sizeof(*scratch));
    *mean_loss=result;
    free(scratch);
    return true;
fail:
    free(scratch);
    return false;
}
