#include "silhouette_score.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double points[]={0,1,5,6};
    const uint32_t labels[]={0,0,1,1};
    const uint32_t noncontiguous[]={1,1,3,3};
    const uint32_t singleton[]={0,0,1,2};
    const uint32_t all_singleton[]={0,1,2,3};
    const uint32_t one_cluster[]={0,0,0,0};
    const uint32_t invalid_labels[]={0,0,1,4};
    const double equal[]={0,0,0,0};
    const double nonfinite[]={0,1,NAN,6};
    const double overflow[]={-1.7e308,1.7e308,0,1};
    double value=-9;
    assert(silhouette_score(points,labels,4,1,&value));
    assert(fabs(value-79.0/99.0)<1e-12);
    assert(silhouette_score(points,noncontiguous,4,1,&value));
    assert(fabs(value-79.0/99.0)<1e-12);
    assert(silhouette_score(equal,labels,4,1,&value) && value==0.0);
    assert(silhouette_score(points,singleton,4,1,&value) && isfinite(value));
    value=-9;
    assert(!silhouette_score(points,all_singleton,4,1,&value));
    assert(!silhouette_score(points,one_cluster,4,1,&value));
    assert(!silhouette_score(points,invalid_labels,4,1,&value));
    assert(!silhouette_score(nonfinite,labels,4,1,&value));
    assert(!silhouette_score(overflow,labels,4,1,&value));
    assert(!silhouette_score(points,labels,1,1,&value));
    assert(!silhouette_score(points,labels,4,0,&value));
    assert(!silhouette_score(points,labels,4,1,NULL));
    assert(value==-9);
    puts("C silhouette score tests passed");
    return 0;
}
