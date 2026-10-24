#include "silhouette_score.h"
#include <math.h>
#include <stdlib.h>

bool silhouette_score(const double *points,const uint32_t *labels,
                      size_t n,size_t d,double *result) {
    if (!points || !labels || !result || n<2 || n>128 || d==0 || d>32)
        return false;
    size_t counts[128]={0};
    size_t clusters=0;
    for (size_t i=0;i<n;++i) {
        if ((size_t)labels[i]>=n) return false;
        for (size_t j=0;j<d;++j)
            if (!isfinite(points[i*d+j])) return false;
        if (counts[labels[i]]==0) ++clusters;
        ++counts[labels[i]];
    }
    if (clusters<2 || clusters>=n) return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        if (counts[labels[i]]==1) continue;
        double sums[128]={0.0};
        for (size_t j=0;j<n;++j) {
            if (j==i) continue;
            double distance=0.0;
            for (size_t col=0;col<d;++col) {
                const double delta=points[i*d+col]-points[j*d+col];
                if (!isfinite(delta)) return false;
                distance=hypot(distance,delta);
                if (!isfinite(distance)) return false;
            }
            sums[labels[j]]+=distance;
            if (!isfinite(sums[labels[j]])) return false;
        }
        const double a=sums[labels[i]]/(double)(counts[labels[i]]-1);
        double b=INFINITY;
        for (size_t c=0;c<n;++c) {
            if (c==labels[i] || counts[c]==0) continue;
            const double average=sums[c]/(double)counts[c];
            if (average<b) b=average;
        }
        if (!isfinite(a) || !isfinite(b)) return false;
        const double denom=fmax(a,b);
        const double sample=denom==0.0?0.0:(b-a)/denom;
        if (!isfinite(sample)) return false;
        total+=sample;
        if (!isfinite(total)) return false;
    }
    const double score=total/(double)n;
    if (!isfinite(score) || score<-1.0-1e-12 || score>1.0+1e-12)
        return false;
    *result=fmax(-1.0,fmin(1.0,score));
    return true;
}
