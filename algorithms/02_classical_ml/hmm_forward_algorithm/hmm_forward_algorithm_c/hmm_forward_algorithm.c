#include "hmm_forward_algorithm.h"
#include <math.h>

static bool pair_is_distribution(double a,double b) {
    return isfinite(a) && isfinite(b) && a>0.0 && b>0.0 &&
           a<=1.0 && b<=1.0 && fabs((a+b)-1.0)<=1e-9;
}
static double logsumexp2(double a,double b) {
    const double high=fmax(a,b);
    const double low=fmin(a,b);
    return high+log1p(exp(low-high));
}
bool hmm_forward_algorithm(const uint8_t *observations,size_t length,
    const double *initial,const double *transition,const double *emission,
    double *log_likelihood) {
    if (!observations || !initial || !transition || !emission ||
        !log_likelihood || length==0 || length>4096 ||
        !pair_is_distribution(initial[0],initial[1]))
        return false;
    for (size_t s=0;s<2;++s) {
        if (!pair_is_distribution(transition[2*s],transition[2*s+1]) ||
            !pair_is_distribution(emission[2*s],emission[2*s+1]))
            return false;
    }
    for (size_t t=0;t<length;++t)
        if (observations[t]>1) return false;
    double prev[2];
    for (size_t s=0;s<2;++s) {
        prev[s]=log(initial[s])+log(emission[2*s+observations[0]]);
        if (!isfinite(prev[s])) return false;
    }
    for (size_t t=1;t<length;++t) {
        double next[2];
        for (size_t s=0;s<2;++s) {
            const double from0=prev[0]+log(transition[s]);
            const double from1=prev[1]+log(transition[2+s]);
            if (!isfinite(from0) || !isfinite(from1)) return false;
            next[s]=logsumexp2(from0,from1)+log(emission[2*s+observations[t]]);
            if (!isfinite(next[s])) return false;
        }
        prev[0]=next[0];prev[1]=next[1];
    }
    const double result=logsumexp2(prev[0],prev[1]);
    if (!isfinite(result) || result>1e-12) return false;
    *log_likelihood=result>0.0?0.0:result;
    return true;
}
