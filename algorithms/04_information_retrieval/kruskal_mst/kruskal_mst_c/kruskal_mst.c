#include "kruskal_mst.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct edge {size_t u,v;double weight;};
static int compare_edge(const void *a,const void *b) {
    const struct edge *x=a,*y=b;
    if (x->weight<y->weight) return -1;
    if (x->weight>y->weight) return 1;
    if (x->u<y->u) return -1;
    if (x->u>y->u) return 1;
    if (x->v<y->v) return -1;
    if (x->v>y->v) return 1;
    return 0;
}
static size_t root(size_t *parent,size_t x) {
    while (parent[x]!=x) {
        parent[x]=parent[parent[x]];
        x=parent[x];
    }
    return x;
}
bool kruskal_mst(const double *weights,size_t n,
                 size_t *u,size_t *v,double *total) {
    if (!weights || !total || n==0 || n>64 ||
        (n>1 && (!u || !v))) return false;
    const size_t max_edges=n*(n-1)/2;
    struct edge *edges=malloc((max_edges?max_edges:1)*sizeof(*edges));
    if (!edges) return false;
    size_t parent[64],sizes[64],best_u[64],best_v[64];
    for (size_t i=0;i<n;++i) {parent[i]=i;sizes[i]=1;}
    size_t count=0;
    for (size_t i=0;i<n;++i) {
        if (weights[i*n+i]!=0.0) goto fail;
        for (size_t j=i+1;j<n;++j) {
            const double a=weights[i*n+j],b=weights[j*n+i];
            if (isnan(a) || isnan(b) || a==-INFINITY || b==-INFINITY ||
                a!=b || (isfinite(a) && (a<0.0 || a>1000000.0)))
                goto fail;
            if (isfinite(a)) edges[count++]=(struct edge){i,j,a};
        }
    }
    qsort(edges,count,sizeof(*edges),compare_edge);
    size_t picked=0;
    double cost=0.0;
    for (size_t e=0;e<count && picked<n-1;++e) {
        size_t a=root(parent,edges[e].u);
        size_t b=root(parent,edges[e].v);
        if (a==b) continue;
        if (sizes[a]<sizes[b]) {size_t temp=a;a=b;b=temp;}
        parent[b]=a;
        sizes[a]+=sizes[b];
        best_u[picked]=edges[e].u;
        best_v[picked]=edges[e].v;
        cost+=edges[e].weight;
        if (!isfinite(cost)) goto fail;
        ++picked;
    }
    if (picked!=n-1) goto fail;
    if (n>1) {
        memcpy(u,best_u,picked*sizeof(*u));
        memcpy(v,best_v,picked*sizeof(*v));
    }
    *total=cost;
    free(edges);
    return true;
fail:
    free(edges);
    return false;
}
