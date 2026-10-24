#include "greedy_k_medoids.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool greedy_k_medoids(const double *points,size_t n,size_t d,size_t k,
                      size_t *medoids) {
    if (!points || !medoids || n==0 || n>128 || d==0 || d>32 ||
        k==0 || k>n) return false;
    for (size_t i=0;i<n*d;++i)
        if (!isfinite(points[i])) return false;
    double *distance=malloc(n*n*sizeof(*distance));
    double *nearest=malloc(n*sizeof(*nearest));
    unsigned char *chosen=calloc(n,sizeof(*chosen));
    size_t *scratch=malloc(k*sizeof(*scratch));
    if (!distance || !nearest || !chosen || !scratch) goto fail;
    for (size_t i=0;i<n;++i) {
        for (size_t j=i;j<n;++j) {
            double metric=0.0;
            for (size_t h=0;h<d;++h) {
                const double difference=points[i*d+h]-points[j*d+h];
                if (!isfinite(difference)) goto fail;
                metric=hypot(metric,difference);
                if (!isfinite(metric)) goto fail;
            }
            distance[i*n+j]=metric;
            distance[j*n+i]=metric;
        }
    }
    for (size_t step=0;step<k;++step) {
        size_t best=0;
        double best_cost=INFINITY;
        bool found=false;
        for (size_t candidate=0;candidate<n;++candidate) {
            if (chosen[candidate]) continue;
            double cost=0.0;
            for (size_t i=0;i<n;++i) {
                const double current=step==0 ? INFINITY : nearest[i];
                const double updated=fmin(current,distance[i*n+candidate]);
                cost+=updated;
                if (!isfinite(cost)) goto fail;
            }
            if (!found || cost<best_cost) {
                best=candidate;
                best_cost=cost;
                found=true;
            }
        }
        if (!found) goto fail;
        scratch[step]=best;
        chosen[best]=1;
        for (size_t i=0;i<n;++i) {
            const double new_distance=distance[i*n+best];
            nearest[i]=step==0 ? new_distance : fmin(nearest[i],new_distance);
        }
    }
    memcpy(medoids,scratch,k*sizeof(*scratch));
    free(distance);free(nearest);free(chosen);free(scratch);
    return true;
fail:
    free(distance);free(nearest);free(chosen);free(scratch);
    return false;
}
