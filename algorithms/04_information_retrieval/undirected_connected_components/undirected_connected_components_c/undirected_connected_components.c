#include "undirected_connected_components.h"
#include <string.h>

bool undirected_connected_components(const uint8_t *adjacency,
                                      size_t n, size_t *labels) {
    if (!adjacency || !labels || n<1 || n>128) return false;
    for (size_t u=0;u<n;++u) for (size_t v=0;v<n;++v)
        if (adjacency[u*n+v]>1 ||
            adjacency[u*n+v]!=adjacency[v*n+u]) return false;
    size_t result[128],queue[128];
    bool seen[128]={false};
    for (size_t start=0;start<n;++start) {
        if (seen[start]) continue;
        size_t head=0,tail=0;
        seen[start]=true;
        result[start]=start;
        queue[tail++]=start;
        while (head<tail) {
            size_t u=queue[head++];
            for (size_t v=0;v<n;++v) {
                if (adjacency[u*n+v]==1 && !seen[v]) {
                    seen[v]=true;
                    result[v]=start;
                    queue[tail++]=v;
                }
            }
        }
    }
    memcpy(labels,result,n*sizeof(*labels));
    return true;
}
