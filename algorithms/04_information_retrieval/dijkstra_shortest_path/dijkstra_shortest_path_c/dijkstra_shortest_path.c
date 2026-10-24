#include "dijkstra_shortest_path.h"
#include <math.h>
#include <string.h>
bool dijkstra_shortest_path(const double *weights,size_t n,size_t source,
                            double *distances) {
    if (!weights || !distances || n==0 || n>128 || source>=n) return false;
    for (size_t i=0;i<n*n;++i)
        if (!isfinite(weights[i]) || weights[i]<0 || weights[i]>1000000)
            return false;
    double dist[128];
    bool visited[128]={false};
    for (size_t i=0;i<n;++i) dist[i]=INFINITY;
    dist[source]=0;
    for (size_t step=0;step<n;++step) {
        size_t u=n;
        for (size_t i=0;i<n;++i) {
            if (!visited[i] && isfinite(dist[i]) &&
                (u==n || dist[i]<dist[u])) u=i;
        }
        if (u==n) break;
        visited[u]=true;
        for (size_t v=0;v<n;++v) {
            const double w=weights[u*n+v];
            if (w==0 || visited[v]) continue;
            const double candidate=dist[u]+w;
            if (!isfinite(candidate)) return false;
            if (candidate<dist[v]) dist[v]=candidate;
        }
    }
    for (size_t i=0;i<n;++i) if (!isfinite(dist[i])) dist[i]=-1;
    memcpy(distances,dist,n*sizeof(*distances));
    return true;
}
