#include "nearest_centroid_classifier.h"
#include <math.h>
#include <string.h>

bool nearest_centroid_classifier(const double *features,const uint8_t *labels,
                                 size_t n,size_t d,double *centroids) {
    if (!features || !labels || !centroids || n<2 || n>4096 || d==0 || d>64)
        return false;
    double sums[128]={0.0},members[2]={0.0,0.0};
    for (size_t i=0;i<n;++i) {
        if (labels[i]>1) return false;
        const unsigned c=labels[i];
        ++members[c];
        for (size_t j=0;j<d;++j) {
            const double value=features[i*d+j];
            if (!isfinite(value)) return false;
            sums[c*d+j]+=value;
            if (!isfinite(sums[c*d+j])) return false;
        }
    }
    if (members[0]==0.0 || members[1]==0.0) return false;
    for (size_t c=0;c<2;++c) {
        for (size_t j=0;j<d;++j) {
            sums[c*d+j]/=members[c];
            if (!isfinite(sums[c*d+j])) return false;
        }
    }
    memcpy(centroids,sums,2*d*sizeof(*centroids));
    return true;
}
bool nearest_centroid_classifier_predict(const double *centroids,
             const double *query,size_t d,uint8_t *label) {
    if (!centroids || !query || !label || d==0 || d>64) return false;
    double distances[2]={0.0,0.0};
    for (size_t j=0;j<d;++j) {
        if (!isfinite(query[j])) return false;
        for (size_t c=0;c<2;++c) {
            const double center=centroids[c*d+j];
            if (!isfinite(center)) return false;
            const double delta=query[j]-center;
            if (!isfinite(delta)) return false;
            distances[c]=hypot(distances[c],delta);
            if (!isfinite(distances[c])) return false;
        }
    }
    *label=(uint8_t)(distances[1]<distances[0]);
    return true;
}
