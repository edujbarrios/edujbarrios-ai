#include "pagerank.h"
#include <math.h>
#include <string.h>

bool pagerank(const double *weights,size_t n,double *scores) {
    if (!weights || !scores || n==0 || n>64) return false;
    double out[64]={0.0},rank[64],next[64];
    for (size_t i=0;i<n;++i) {
        for (size_t j=0;j<n;++j) {
            const double w=weights[i*n+j];
            if (!isfinite(w) || w<0.0 || w>1000000.0) return false;
            out[i]+=w;
            if (!isfinite(out[i])) return false;
        }
        rank[i]=1.0/(double)n;
    }
    for (size_t step=0;step<100;++step) {
        for (size_t j=0;j<n;++j) next[j]=0.15/(double)n;
        for (size_t i=0;i<n;++i) {
            if (out[i]==0.0) {
                const double share=0.85*rank[i]/(double)n;
                if (!isfinite(share)) return false;
                for (size_t j=0;j<n;++j) next[j]+=share;
            } else {
                const double factor=0.85*rank[i]/out[i];
                if (!isfinite(factor)) return false;
                for (size_t j=0;j<n;++j) {
                    const double contribution=factor*weights[i*n+j];
                    next[j]+=contribution;
                    if (!isfinite(contribution) || !isfinite(next[j])) return false;
                }
            }
        }
        for (size_t j=0;j<n;++j) {
            if (!isfinite(next[j]) || next[j]<0.0) return false;
            rank[j]=next[j];
        }
    }
    memcpy(scores,rank,n*sizeof(*scores));
    return true;
}
