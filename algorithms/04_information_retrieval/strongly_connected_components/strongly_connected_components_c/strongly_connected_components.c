#include "strongly_connected_components.h"
#include <string.h>

static void finish_dfs(const int *a, size_t n, size_t u,
                       bool *seen, size_t *order, size_t *count) {
    seen[u] = true;
    for (size_t v = 0; v < n; ++v)
        if (a[u*n+v] == 1 && !seen[v])
            finish_dfs(a,n,v,seen,order,count);
    order[(*count)++] = u;
}

static void transpose_dfs(const int *a, size_t n, size_t u,
                          size_t root, size_t *component) {
    component[u] = root;
    for (size_t v = 0; v < n; ++v)
        if (a[v*n+u] == 1 && component[v] == n)
            transpose_dfs(a,n,v,root,component);
}

bool strongly_connected_components(const int *adjacency, size_t n,
                                   size_t *labels) {
    if (!adjacency || !labels || n == 0 || n > 128) return false;
    for (size_t i = 0; i < n*n; ++i)
        if (adjacency[i] != 0 && adjacency[i] != 1) return false;

    bool seen[128] = {false};
    size_t order[128], count = 0, component[128], minimum[128];
    for (size_t u = 0; u < n; ++u)
        if (!seen[u]) finish_dfs(adjacency,n,u,seen,order,&count);
    for (size_t i = 0; i < n; ++i) {
        component[i] = n;
        minimum[i] = n;
    }
    for (size_t k = n; k > 0; --k) {
        size_t u = order[k-1];
        if (component[u] == n)
            transpose_dfs(adjacency,n,u,u,component);
    }
    for (size_t i = 0; i < n; ++i)
        if (i < minimum[component[i]]) minimum[component[i]] = i;
    size_t result[128];
    for (size_t i = 0; i < n; ++i) result[i] = minimum[component[i]];
    memcpy(labels,result,n*sizeof(*labels));
    return true;
}
