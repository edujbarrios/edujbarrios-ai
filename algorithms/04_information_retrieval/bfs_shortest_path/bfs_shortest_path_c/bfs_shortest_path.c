#include "bfs_shortest_path.h"
#include <string.h>

bool bfs_shortest_path(const int *weights, size_t n, size_t source,
                       int *distances) {
    if (!weights || !distances || n < 1 || n > 128 || source >= n) return false;
    for (size_t i = 0; i < n*n; ++i)
        if (weights[i] != 0 && weights[i] != 1) return false;

    int dist[128];
    size_t queue[128], head = 0, tail = 0;
    for (size_t i = 0; i < n; ++i) dist[i] = -1;
    dist[source] = 0;
    queue[tail++] = source;
    while (head < tail) {
        size_t u = queue[head++];
        for (size_t v = 0; v < n; ++v) {
            if (weights[u*n+v] == 1 && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                queue[tail++] = v;
            }
        }
    }
    memcpy(distances, dist, n*sizeof(*distances));
    return true;
}
