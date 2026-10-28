#include "bipartite_graph_check.h"

bool bipartite_graph_check(const uint8_t *adjacency, size_t n, bool *out) {
    if (!adjacency || !out || n<1 || n>128) return false;
    for(size_t u=0;u<n;++u) for(size_t v=0;v<n;++v)
        if(adjacency[u*n+v]>1 || adjacency[u*n+v]!=adjacency[v*n+u])
            return false;
    int color[128];
    size_t queue[128];
    for(size_t i=0;i<n;++i) color[i]=-1;
    for(size_t start=0;start<n;++start) {
        if(color[start]!=-1) continue;
        size_t head=0,tail=0;
        color[start]=0;
        queue[tail++]=start;
        while(head<tail) {
            size_t u=queue[head++];
            for(size_t v=0;v<n;++v) {
                if(adjacency[u*n+v]==0) continue;
                if(color[v]==-1) {
                    color[v]=1-color[u];
                    queue[tail++]=v;
                } else if(color[v]==color[u]) {
                    *out=false;
                    return true;
                }
            }
        }
    }
    *out=true;
    return true;
}
