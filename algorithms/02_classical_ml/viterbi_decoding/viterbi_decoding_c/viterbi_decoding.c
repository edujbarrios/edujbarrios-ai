#include "viterbi_decoding.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static bool normalized_pair(double p,double q) {
    return isfinite(p) && isfinite(q) && p>0.0 && q>0.0 &&
           p<=1.0 && q<=1.0 && fabs((p+q)-1.0)<=1e-9;
}
bool viterbi_decoding(const uint8_t *observations,size_t length,
    const double *initial,const double *transition,const double *emission,
    uint8_t *states,double *log_probability) {
    if (!observations || !initial || !transition || !emission ||
        !states || !log_probability || length==0 || length>4096)
        return false;
    if (!normalized_pair(initial[0],initial[1])) return false;
    for (size_t i=0;i<2;++i) {
        if (!normalized_pair(transition[2*i],transition[2*i+1]) ||
            !normalized_pair(emission[2*i],emission[2*i+1]))
            return false;
    }
    for (size_t t=0;t<length;++t)
        if (observations[t]>1) return false;
    uint8_t *back=malloc(length*2*sizeof(*back));
    uint8_t *path=malloc(length*sizeof(*path));
    if (!back || !path){free(back);free(path);return false;}
    double previous[2],current[2];
    for (size_t s=0;s<2;++s) {
        previous[s]=log(initial[s])+log(emission[2*s+observations[0]]);
        if (!isfinite(previous[s])) goto fail;
    }
    for (size_t t=1;t<length;++t) {
        for (size_t s=0;s<2;++s) {
            const double from_zero=previous[0]+log(transition[s]);
            const double from_one=previous[1]+log(transition[2+s]);
            const uint8_t predecessor=(uint8_t)(from_one>from_zero);
            const double best=predecessor?from_one:from_zero;
            current[s]=best+log(emission[2*s+observations[t]]);
            if (!isfinite(from_zero) || !isfinite(from_one) ||
                !isfinite(current[s])) goto fail;
            back[t*2+s]=predecessor;
        }
        previous[0]=current[0];
        previous[1]=current[1];
    }
    uint8_t state=(uint8_t)(previous[1]>previous[0]);
    const double best_log=previous[state];
    for (size_t step=length;step>0;--step) {
        const size_t t=step-1;
        path[t]=state;
        if (t>0) state=back[t*2+state];
    }
    memcpy(states,path,length*sizeof(*path));
    *log_probability=best_log;
    free(back);free(path);
    return true;
fail:
    free(back);free(path);
    return false;
}
