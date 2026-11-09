#include "euclidean_radius_search.h"
#include <math.h>
#include <stdlib.h>

typedef struct { double distance; size_t index; } Match;
static int compare_match(const void *a,const void *b) {
    const Match *x=(const Match *)a, *y=(const Match *)b;
    if (x->distance<y->distance) return -1;
    if (x->distance>y->distance) return 1;
    return (x->index>y->index)-(x->index<y->index);
}
bool euclidean_radius_search(const double *query,const double *candidates,
                             size_t n,size_t d,double radius,
                             size_t indices[128],size_t *count_out) {
    if (!query || !candidates || !indices || !count_out ||
        n<1 || n>128 || d<1 || d>32 || !isfinite(radius) ||
        radius<0.0 || radius>10000.0 ||
        (void *)indices==(void *)count_out) return false;
    for(size_t j=0;j<d;++j)
        if (!isfinite(query[j]) || fabs(query[j])>1000.0) return false;
    for(size_t i=0;i<n*d;++i)
        if (!isfinite(candidates[i]) || fabs(candidates[i])>1000.0) return false;
    Match matches[128];
    size_t count=0;
    for(size_t i=0;i<n;++i) {
        double distance=0.0;
        for(size_t j=0;j<d;++j)
            distance=hypot(distance,query[j]-candidates[i*d+j]);
        if(distance<=radius) {
            matches[count].distance=distance;
            matches[count].index=i;
            ++count;
        }
    }
    qsort(matches,count,sizeof(matches[0]),compare_match);
    for(size_t i=0;i<count;++i)indices[i]=matches[i].index;
    *count_out=count;
    return true;
}
