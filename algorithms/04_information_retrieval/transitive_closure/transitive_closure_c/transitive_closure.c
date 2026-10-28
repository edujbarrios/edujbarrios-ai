#include "transitive_closure.h"
#include <string.h>

bool transitive_closure(const uint8_t *adjacency, size_t n, uint8_t *output) {
    if (!adjacency || !output || n == 0 || n > 64) return false;
    for (size_t i=0;i<n*n;++i)
        if (adjacency[i] > 1) return false;
    uint8_t reach[64*64];
    memcpy(reach, adjacency, n*n*sizeof(*reach));
    for (size_t i=0;i<n;++i) reach[i*n+i]=1;
    for (size_t k=0;k<n;++k)
        for (size_t i=0;i<n;++i)
            for (size_t j=0;j<n;++j)
                if (reach[i*n+k] && reach[k*n+j]) reach[i*n+j]=1;
    memcpy(output, reach, n*n*sizeof(*output));
    return true;
}
