#include "euclidean_top_k.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
struct pair {double distance;size_t index;};
static int compare_pair(const void *pa,const void *pb) {
    const struct pair *a=pa,*b=pb;
    if (a->distance<b->distance) return -1;
    if (a->distance>b->distance) return 1;
    if (a->index<b->index) return -1;
    if (a->index>b->index) return 1;
    return 0;
}
bool euclidean_top_k(const double *query,size_t d,const double *candidates,
                     size_t n,size_t k,size_t *indices) {
    if (!query || !candidates || !indices || n==0 || d==0 ||
        k==0 || k>n || n>SIZE_MAX/d ||
        n>SIZE_MAX/sizeof(struct pair)) return false;
    for (size_t j=0;j<d;++j) if (!isfinite(query[j])) return false;
    for (size_t i=0;i<n*d;++i) if (!isfinite(candidates[i])) return false;
    struct pair *pairs=malloc(n*sizeof(*pairs));
    if (!pairs) return false;
    for (size_t i=0;i<n;++i) {
        double distance=0.0;
        for (size_t j=0;j<d;++j) {
            const double diff=query[j]-candidates[i*d+j];
            if (!isfinite(diff)) {free(pairs);return false;}
            distance=hypot(distance,diff);
            if (!isfinite(distance)) {free(pairs);return false;}
        }
        pairs[i].distance=distance;
        pairs[i].index=i;
    }
    qsort(pairs,n,sizeof(*pairs),compare_pair);
    for (size_t i=0;i<k;++i) indices[i]=pairs[i].index;
    free(pairs);
    return true;
}
