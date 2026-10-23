#include "confusion_matrix.h"

bool confusion_matrix(const uint8_t *actual, const uint8_t *predicted,
                      size_t n, size_t output[4]) {
    if (!actual || !predicted || !output || n == 0) return false;
    size_t counts[4] = {0,0,0,0};
    for (size_t i=0;i<n;++i) {
        if (actual[i]>1 || predicted[i]>1) return false;
        const size_t index=(size_t)actual[i]*2+(size_t)predicted[i];
        ++counts[index];
    }
    for (size_t i=0;i<4;++i) output[i]=counts[i];
    return true;
}
