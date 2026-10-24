#include "hinge_loss.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool hinge_loss(const double *scores,const int8_t *labels,size_t n,
                double *mean_loss,double *gradients) {
    if (!scores || !labels || !mean_loss || !gradients ||
        n==0 || n>SIZE_MAX/sizeof(double)) return false;
    double *scratch=malloc(n*sizeof(*scratch));
    if (!scratch) return false;
    double accumulated=0.0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(scores[i]) ||
            (labels[i]!=INT8_C(-1) && labels[i]!=INT8_C(1))) {
            free(scratch);return false;
        }
        const double margin=(double)labels[i]*scores[i];
        const double deficit=1.0-margin;
        if (!isfinite(margin) || !isfinite(deficit)) {
            free(scratch);return false;
        }
        const double loss=deficit>0.0?deficit:0.0;
        accumulated+=loss;
        scratch[i]=margin<1.0?-(double)labels[i]/(double)n:0.0;
        if (!isfinite(accumulated) || !isfinite(scratch[i])) {
            free(scratch);return false;
        }
    }
    const double result=accumulated/(double)n;
    if (!isfinite(result)) {free(scratch);return false;}
    memcpy(gradients,scratch,n*sizeof(*scratch));
    *mean_loss=result;
    free(scratch);
    return true;
}
