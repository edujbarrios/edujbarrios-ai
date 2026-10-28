#include "prim_mst.h"
#include <math.h>

bool prim_mst(const double *weights, size_t n, double *total) {
    if (!weights || !total || n < 1 || n > 128) return false;
    for (size_t u = 0; u < n; ++u) {
        if (weights[u*n+u] != 0.0) return false;
        for (size_t v = 0; v < n; ++v) {
            const double w = weights[u*n+v];
            if (!isfinite(w) || w < 0.0 || w > 1000000.0 ||
                w != weights[v*n+u]) return false;
        }
    }
    bool inside[128] = {false};
    double best[128];
    for (size_t v = 0; v < n; ++v) best[v] = INFINITY;
    best[0] = 0.0;
    double sum = 0.0;
    for (size_t step = 0; step < n; ++step) {
        size_t u = n;
        for (size_t v = 0; v < n; ++v) {
            if (!inside[v] && isfinite(best[v]) &&
                (u == n || best[v] < best[u])) u = v;
        }
        if (u == n) return false;
        inside[u] = true;
        sum += best[u];
        for (size_t v = 0; v < n; ++v) {
            const double w = weights[u*n+v];
            if (!inside[v] && w > 0.0 && w < best[v]) best[v] = w;
        }
    }
    if (!isfinite(sum)) return false;
    *total = sum;
    return true;
}
