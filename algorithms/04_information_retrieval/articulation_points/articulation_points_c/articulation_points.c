#include "articulation_points.h"

typedef struct {
    const uint8_t *a;
    size_t n;
    int disc[64], low[64], parent[64], clock;
    bool cut[64];
} DfsState;

static void visit(DfsState *s, size_t u) {
    s->disc[u] = s->low[u] = ++s->clock;
    size_t children = 0;
    for (size_t v = 0; v < s->n; ++v) {
        if (s->a[u*s->n+v] == 0) continue;
        if (s->disc[v] == 0) {
            s->parent[v] = (int)u;
            ++children;
            visit(s,v);
            if (s->low[v] < s->low[u]) s->low[u] = s->low[v];
            if (s->parent[u] != -1 && s->low[v] >= s->disc[u])
                s->cut[u] = true;
        } else if ((int)v != s->parent[u] && s->disc[v] < s->low[u]) {
            s->low[u] = s->disc[v];
        }
    }
    if (s->parent[u] == -1 && children > 1) s->cut[u] = true;
}

bool articulation_points(
    const uint8_t *adjacency, size_t n, size_t cut_vertices[64],
    size_t *count_out) {
    if (!adjacency || !cut_vertices || !count_out ||
        (void *)cut_vertices == (void *)count_out || n < 1 || n > 64)
        return false;
    for (size_t u=0;u<n;++u) {
        for (size_t v=0;v<n;++v) {
            if (adjacency[u*n+v] > 1 ||
                (u==v && adjacency[u*n+v] != 0) ||
                adjacency[u*n+v] != adjacency[v*n+u]) return false;
        }
    }
    DfsState s = {0};
    s.a=adjacency;
    s.n=n;
    for (size_t u=0;u<n;++u) s.parent[u]=-1;
    for (size_t u=0;u<n;++u) if (!s.disc[u]) visit(&s,u);
    size_t count=0;
    for (size_t u=0;u<n;++u) if (s.cut[u]) cut_vertices[count++]=u;
    *count_out=count;
    return true;
}
