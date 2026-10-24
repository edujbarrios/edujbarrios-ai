#include "farthest_first_traversal.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool farthest_first_traversal(const double *points,size_t n,size_t d,
                              size_t k,size_t *indices) {
    if (!points || !indices || n==0 || d==0 || k==0 || k>n ||
        n>SIZE_MAX/d || n>SIZE_MAX/sizeof(double) ||
        k>SIZE_MAX/sizeof(size_t)) return false;
    for (size_t i=0;i<n*d;++i) if (!isfinite(points[i])) return false;
    double *nearest=malloc(n*sizeof(*nearest));
    bool *used=calloc(n,sizeof(*used));
    size_t *centers=malloc(k*sizeof(*centers));
    if (!nearest || !used || !centers) {
        free(nearest);free(used);free(centers);return false;
    }
    for (size_t i=0;i<n;++i) nearest[i]=INFINITY;
    for (size_t step=0;step<k;++step) {
        size_t chosen=0;
        if (step!=0) {
            bool found=false;
            double best=-1.0;
            for (size_t i=0;i<n;++i) {
                if (!used[i] && (!found || nearest[i]>best)) {
                    chosen=i;best=nearest[i];found=true;
                }
            }
            if (!found) goto fail;
        }
        centers[step]=chosen;
        used[chosen]=true;
        for (size_t i=0;i<n;++i) {
            if (used[i]) continue;
            double distance=0.0;
            for (size_t j=0;j<d;++j) {
                const double delta=points[i*d+j]-points[chosen*d+j];
                if (!isfinite(delta)) goto fail;
                distance=hypot(distance,delta);
                if (!isfinite(distance)) goto fail;
            }
            if (distance<nearest[i]) nearest[i]=distance;
        }
    }
    memcpy(indices,centers,k*sizeof(*centers));
    free(nearest);free(used);free(centers);
    return true;
fail:
    free(nearest);free(used);free(centers);
    return false;
}
