#include "astar_shortest_path.h"
#include <math.h>
#include <stdint.h>

bool astar_shortest_path(const double *weights,const double *heuristic,
    size_t n,size_t source,size_t target,double *distance,
    size_t *path,size_t *path_length) {
    if (!weights || !heuristic || !distance || !path || !path_length ||
        n==0 || n>64 || source>=n || target>=n) return false;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(heuristic[i]) || heuristic[i]<0 ||
            heuristic[i]>1000000.0) return false;
    }
    if (heuristic[target]!=0.0) return false;
    for (size_t u=0;u<n;++u) {
        for (size_t v=0;v<n;++v) {
            const double w=weights[u*n+v];
            if (!isfinite(w) || w<0 || w>1000000.0) return false;
            if (w>0 && heuristic[u]>w+heuristic[v]+1e-9) return false;
        }
    }
    double g[64];
    size_t parent[64];
    bool closed[64]={false};
    for (size_t i=0;i<n;++i) {g[i]=INFINITY;parent[i]=SIZE_MAX;}
    g[source]=0.0;
    bool reached=false;
    for (size_t step=0;step<n;++step) {
        size_t current=SIZE_MAX;
        double best=INFINITY;
        for (size_t i=0;i<n;++i) {
            if (closed[i] || !isfinite(g[i])) continue;
            const double f=g[i]+heuristic[i];
            if (!isfinite(f)) return false;
            if (current==SIZE_MAX || f<best) {current=i;best=f;}
        }
        if (current==SIZE_MAX) break;
        if (current==target) {reached=true;break;}
        closed[current]=true;
        for (size_t v=0;v<n;++v) {
            const double w=weights[current*n+v];
            if (w==0.0 || closed[v]) continue;
            const double tentative=g[current]+w;
            if (!isfinite(tentative)) return false;
            if (tentative<g[v]) {g[v]=tentative;parent[v]=current;}
        }
    }
    if (!reached) {
        *distance=-1.0;
        *path_length=0;
        return true;
    }
    size_t reversed[64],count=0,node=target;
    while (count<n) {
        reversed[count++]=node;
        if (node==source) break;
        if (parent[node]==SIZE_MAX) return false;
        node=parent[node];
    }
    if (reversed[count-1]!=source) return false;
    for (size_t i=0;i<count;++i) path[i]=reversed[count-i-1];
    *distance=g[target];
    *path_length=count;
    return true;
}
