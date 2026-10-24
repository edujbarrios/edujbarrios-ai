#include "floyd_warshall.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool floyd_warshall(const double *adjacency,size_t n,double *distances) {
    if (!adjacency || !distances || n==0 || n>64) return false;
    const size_t len=n*n;
    for (size_t i=0;i<len;++i) {
        const double v=adjacency[i];
        if (isnan(v) || v==-INFINITY ||
            (isfinite(v) && (v< -1000000.0 || v>1000000.0))) return false;
    }
    for (size_t i=0;i<n;++i) if (adjacency[i*n+i]!=0.0) return false;
    double *scratch=malloc(len*sizeof(*scratch));
    if (!scratch) return false;
    memcpy(scratch,adjacency,len*sizeof(*scratch));
    for (size_t k=0;k<n;++k) {
        for (size_t i=0;i<n;++i) {
            if (!isfinite(scratch[i*n+k])) continue;
            for (size_t j=0;j<n;++j) {
                if (!isfinite(scratch[k*n+j])) continue;
                const double via=scratch[i*n+k]+scratch[k*n+j];
                if (!isfinite(via)) {free(scratch);return false;}
                if (via<scratch[i*n+j]) scratch[i*n+j]=via;
            }
        }
    }
    for (size_t i=0;i<n;++i) {
        if (scratch[i*n+i]<0.0) {free(scratch);return false;}
    }
    memcpy(distances,scratch,len*sizeof(*scratch));
    free(scratch);
    return true;
}
