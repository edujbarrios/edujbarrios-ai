#include "matthews_correlation_coefficient.h"
#include <math.h>

bool matthews_correlation_coefficient(const uint8_t *truth,
    const uint8_t *predictions,size_t n,double *result) {
    if (!truth || !predictions || !result || n==0) return false;
    double tp=0,tn=0,fp=0,fn=0;
    for (size_t i=0;i<n;++i) {
        if (truth[i]>1 || predictions[i]>1) return false;
        if (truth[i]==1 && predictions[i]==1) ++tp;
        else if (truth[i]==0 && predictions[i]==0) ++tn;
        else if (truth[i]==0) ++fp;
        else ++fn;
    }
    const double total=(double)n;
    tp/=total;tn/=total;fp/=total;fn/=total;
    const double product=(tp+fp)*(tp+fn)*(tn+fp)*(tn+fn);
    if (!isfinite(product) || product<0) return false;
    double score=0;
    if (product>0) score=(tp*tn-fp*fn)/sqrt(product);
    if (!isfinite(score) || score<-1.0-1e-12 || score>1.0+1e-12)
        return false;
    *result=fmax(-1.0,fmin(1.0,score));
    return true;
}
