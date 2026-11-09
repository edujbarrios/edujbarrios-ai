#include "undirected_graph_bridges.h"

typedef struct {
    const uint8_t *a;
    size_t n;
    int disc[64], low[64], parent[64], clock;
    bool bridge[64][64];
} BridgeState;

static void visit(BridgeState *s, size_t u) {
    s->disc[u] = s->low[u] = ++s->clock;
    for (size_t v=0;v<s->n;++v) {
        if (s->a[u*s->n+v] == 0) continue;
        if (s->disc[v] == 0) {
            s->parent[v] = (int)u;
            visit(s,v);
            if (s->low[v] < s->low[u]) s->low[u] = s->low[v];
            if (s->low[v] > s->disc[u]) {
                const size_t lo=u<v?u:v, hi=u<v?v:u;
                s->bridge[lo][hi] = true;
            }
        } else if ((int)v != s->parent[u] && s->disc[v] < s->low[u]) {
            s->low[u] = s->disc[v];
        }
    }
}

bool undirected_graph_bridges(const uint8_t *adjacency, size_t n,
                              size_t endpoints[4032], size_t *bridge_count_out) {
    if (!adjacency || !endpoints || !bridge_count_out ||
        (void *)endpoints == (void *)bridge_count_out || n<1 || n>64)
        return false;
    for (size_t u=0;u<n;++u) {
        for (size_t v=0;v<n;++v) {
            if (adjacency[u*n+v]>1 ||
                (u==v && adjacency[u*n+v]!=0) ||
                adjacency[u*n+v]!=adjacency[v*n+u]) return false;
        }
    }
    BridgeState s={0};
    s.a=adjacency;
    s.n=n;
    for (size_t i=0;i<n;++i) s.parent[i]=-1;
    for (size_t i=0;i<n;++i) if (!s.disc[i]) visit(&s,i);
    size_t count=0;
    for (size_t u=0;u<n;++u) {
        for (size_t v=u+1;v<n;++v) {
            if (s.bridge[u][v]) {
                endpoints[2*count]=u;
                endpoints[2*count+1]=v;
                ++count;
            }
        }
    }
    *bridge_count_out=count;
    return true;
}
