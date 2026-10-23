#include "f1_score.h"
#include <math.h>

bool f1_score(const uint8_t *actual, const uint8_t *predicted,
              size_t n, double *score) {
    if (!actual || !predicted || !score || n==0) return false;
    size_t tp=0,fp=0,fn=0;
    for (size_t i=0; i<n; ++i) {
        if (actual[i]>1 || predicted[i]>1) return false;
        if (actual[i]==1 && predicted[i]==1) ++tp;
        else if (actual[i]==0 && predicted[i]==1) ++fp;
        else if (actual[i]==1 && predicted[i]==0) ++fn;
    }
    const double numerator=2.0*(double)tp;
    const double denominator=numerator+(double)fp+(double)fn;
    const double result=denominator==0.0?0.0:numerator/denominator;
    if (!isfinite(result) || result<0.0 || result>1.0) return false;
    *score=result;
    return true;
}
