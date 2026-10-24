#include "dbscan.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool dbscan(const double *points,size_t n,size_t d,double epsilon,
            size_t min_samples,int *labels) {
    if (!points || !labels || n==0 || n>1024 || d==0 || d>64 ||
        min_samples==0 || min_samples>n || !isfinite(epsilon) ||
        epsilon<=0 || n>SIZE_MAX/n || n>SIZE_MAX/d) return false;
    for (size_t i=0;i<n*d;++i)
        if (!isfinite(points[i])) return false;

    unsigned char *near=calloc(n*n,sizeof(*near));
    size_t *counts=calloc(n,sizeof(*counts));
    unsigned char *visited=calloc(n,sizeof(*visited));
    unsigned char *enqueued=calloc(n,sizeof(*enqueued));
    size_t *queue=malloc(n*sizeof(*queue));
    int *scratch=malloc(n*sizeof(*scratch));
    if (!near || !counts || !visited || !enqueued || !queue || !scratch)
        goto fail;
    for (size_t i=0;i<n;++i) scratch[i]=-1;

    for (size_t i=0;i<n;++i) {
        for (size_t j=i;j<n;++j) {
            double distance=0.0;
            for (size_t col=0;col<d;++col) {
                const double diff=points[i*d+col]-points[j*d+col];
                if (!isfinite(diff)) goto fail;
                distance=hypot(distance,diff);
                if (!isfinite(distance)) goto fail;
            }
            if (distance<=epsilon) {
                near[i*n+j]=1;
                near[j*n+i]=1;
                ++counts[i];
                if (i!=j) ++counts[j];
            }
        }
    }
    int cluster=0;
    for (size_t i=0;i<n;++i) {
        if (visited[i]) continue;
        visited[i]=1;
        if (counts[i]<min_samples) continue;
        scratch[i]=cluster;
        memset(enqueued,0,n*sizeof(*enqueued));
        size_t front=0,back=0;
        for (size_t j=0;j<n;++j) {
            if (j!=i && near[i*n+j]) {
                queue[back++]=j;enqueued[j]=1;
            }
        }
        while (front<back) {
            const size_t j=queue[front++];
            if (!visited[j]) {
                visited[j]=1;
                if (counts[j]>=min_samples) {
                    for (size_t h=0;h<n;++h) {
                        if (near[j*n+h] && !enqueued[h] && !visited[h]) {
                            queue[back++]=h;enqueued[h]=1;
                        }
                    }
                }
            }
            if (scratch[j]==-1) scratch[j]=cluster;
        }
        ++cluster;
    }
    memcpy(labels,scratch,n*sizeof(*scratch));
    free(near);free(counts);free(visited);free(enqueued);free(queue);free(scratch);
    return true;
fail:
    free(near);free(counts);free(visited);free(enqueued);free(queue);free(scratch);
    return false;
}
