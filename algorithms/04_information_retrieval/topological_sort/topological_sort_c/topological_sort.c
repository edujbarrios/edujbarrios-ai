#include "topological_sort.h"
#include <string.h>

bool topological_sort(const int *weights, size_t n, size_t *order) {
    if (!weights || !order || n < 1 || n > 128) return false;
    size_t indegree[128] = {0}, result[128];
    bool used[128] = {false};
    for (size_t u = 0; u < n; ++u) {
        for (size_t v = 0; v < n; ++v) {
            const int edge = weights[u*n+v];
            if (edge != 0 && edge != 1) return false;
            if (edge == 1) ++indegree[v];
        }
    }
    for (size_t k = 0; k < n; ++k) {
        size_t u = n;
        for (size_t v = 0; v < n; ++v) {
            if (!used[v] && indegree[v] == 0) { u = v; break; }
        }
        if (u == n) return false;
        result[k] = u;
        used[u] = true;
        for (size_t v = 0; v < n; ++v) {
            if (weights[u*n+v] == 1) --indegree[v];
        }
    }
    memcpy(order,result,n*sizeof(*order));
    return true;
}
