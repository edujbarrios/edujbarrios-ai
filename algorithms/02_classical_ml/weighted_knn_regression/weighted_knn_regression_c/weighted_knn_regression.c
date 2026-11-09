#include "weighted_knn_regression.h"
#include <math.h>
#include <stdlib.h>

typedef struct {double distance;size_t index;} Neighbor;
static int compare_neighbor(const void *a,const void *b) {
    const Neighbor *x=(const Neighbor *)a,*y=(const Neighbor *)b;
    if(x->distance<y->distance)return -1;
    if(x->distance>y->distance)return 1;
    return (x->index>y->index)-(x->index<y->index);
}
bool weighted_knn_regression(const double *train,const double *targets,
                             const double *query,size_t n,size_t d,size_t k,
                             double *prediction_out) {
    if(!train || !targets || !query || !prediction_out ||
       n<1 || n>128 || d<1 || d>16 || k<1 || k>n ||
       train==prediction_out || targets==prediction_out ||
       query==prediction_out) return false;
    for(size_t i=0;i<n*d;++i)
        if(!isfinite(train[i]) || fabs(train[i])>1000.0)return false;
    for(size_t i=0;i<n;++i)
        if(!isfinite(targets[i]) || fabs(targets[i])>1000.0)return false;
    for(size_t j=0;j<d;++j)
        if(!isfinite(query[j]) || fabs(query[j])>1000.0)return false;
    Neighbor neighbors[128];
    for(size_t i=0;i<n;++i) {
        double distance=0.0;
        for(size_t j=0;j<d;++j)
            distance=hypot(distance,train[i*d+j]-query[j]);
        neighbors[i].distance=distance;
        neighbors[i].index=i;
    }
    qsort(neighbors,n,sizeof(neighbors[0]),compare_neighbor);
    double sum=0.0;
    size_t zeros=0;
    for(size_t i=0;i<k;++i)
        if(neighbors[i].distance==0.0)++zeros;
    if(zeros) {
        for(size_t i=0;i<k;++i)
            if(neighbors[i].distance==0.0)
                sum+=targets[neighbors[i].index]/(double)zeros;
        *prediction_out=sum;
        return true;
    }
    const double closest=neighbors[0].distance;
    double denominator=0.0;
    for(size_t i=0;i<k;++i) {
        const double weight=closest/neighbors[i].distance;
        denominator+=weight;
        sum+=weight*targets[neighbors[i].index];
    }
    const double prediction=sum/denominator;
    if(!isfinite(prediction))return false;
    *prediction_out=prediction;
    return true;
}
