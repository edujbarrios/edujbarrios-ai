#include "bellman_ford_shortest_path.h"
#include <math.h>
#include <string.h>

bool bellman_ford_shortest_path(const BFEdge *edges, size_t m, size_t n,
                                size_t source, double *distances) {
    if (!distances || n < 1 || n > 128 || source >= n ||
        m > 16384 || (m > 0 && !edges)) return false;
    for (size_t i = 0; i < m; ++i) {
        if (edges[i].from >= n || edges[i].to >= n ||
            !isfinite(edges[i].weight) ||
            fabs(edges[i].weight) > 1000000.0) return false;
    }
    double dist[128];
    for (size_t i = 0; i < n; ++i) dist[i] = INFINITY;
    dist[source] = 0.0;
    for (size_t pass = 1; pass < n; ++pass) {
        bool changed = false;
        for (size_t i = 0; i < m; ++i) {
            const BFEdge e = edges[i];
            if (!isfinite(dist[e.from])) continue;
            const double candidate = dist[e.from] + e.weight;
            if (!isfinite(candidate)) return false;
            if (candidate < dist[e.to]) {
                dist[e.to] = candidate;
                changed = true;
            }
        }
        if (!changed) break;
    }
    for (size_t i = 0; i < m; ++i) {
        const BFEdge e = edges[i];
        if (isfinite(dist[e.from]) && dist[e.from] + e.weight < dist[e.to])
            return false;
    }
    memcpy(distances, dist, n * sizeof(*distances));
    return true;
}
