#include "knn_regression.h"
#include <math.h>
#include <stdlib.h>

struct neighbor {double distance;size_t index;};
static int cmp_neighbor(const void *left,const void *right) {
    const struct neighbor *a=left,*b=right;
    if (a->distance<b->distance) return -1;
    if (a->distance>b->distance) return 1;
    if (a->index<b->index) return -1;
    if (a->index>b->index) return 1;
    return 0;
}
bool knn_regression(const double *training,const double *targets,
                    size_t n,size_t d,const double *query,
                    size_t k,double *prediction) {
    if (!training || !targets || !query || !prediction ||
        n==0 || n>4096 || d==0 || d>64 || k==0 || k>n) return false;
    for (size_t j=0;j<d;++j)
        if (!isfinite(query[j])) return false;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(targets[i])) return false;
        for (size_t j=0;j<d;++j)
            if (!isfinite(training[i*d+j])) return false;
    }
    struct neighbor *neighbors=malloc(n*sizeof(*neighbors));
    if (!neighbors) return false;
    for (size_t i=0;i<n;++i) {
        double distance=0.0;
        for (size_t j=0;j<d;++j) {
            const double difference=training[i*d+j]-query[j];
            if (!isfinite(difference)) {free(neighbors);return false;}
            distance=hypot(distance,difference);
            if (!isfinite(distance)) {free(neighbors);return false;}
        }
        neighbors[i].index=i;
        neighbors[i].distance=distance;
    }
    qsort(neighbors,n,sizeof(*neighbors),cmp_neighbor);
    double answer=0.0;
    for (size_t i=0;i<k;++i) {
        const double contribution=targets[neighbors[i].index]/(double)k;
        if (!isfinite(contribution)) {free(neighbors);return false;}
        answer+=contribution;
        if (!isfinite(answer)) {free(neighbors);return false;}
    }
    *prediction=answer;
    free(neighbors);
    return true;
}
