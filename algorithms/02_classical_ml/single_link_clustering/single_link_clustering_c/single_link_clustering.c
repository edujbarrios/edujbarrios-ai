#include "single_link_clustering.h"
#include <stdint.h>

bool single_link_clustering(const int32_t *points, size_t n, size_t k,
                            size_t *labels) {
    if (!points || !labels || n == 0 || n > 32 || k == 0 || k > n)
        return false;
    for (size_t t = 0; t < 2 * n; ++t)
        if (points[t] < -1000 || points[t] > 1000) return false;

    size_t groups[32];
    for (size_t i = 0; i < n; ++i) groups[i] = i;
    for (size_t step = 0; step < n - k; ++step) {
        int64_t best = INT64_MAX;
        size_t best_i = 0, best_j = 0;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (groups[i] == groups[j]) continue;
                const int64_t dx = (int64_t)points[2*i] - points[2*j];
                const int64_t dy = (int64_t)points[2*i+1] - points[2*j+1];
                const int64_t d2 = dx * dx + dy * dy;
                if (d2 < best) {
                    best = d2; best_i = i; best_j = j;
                }
            }
        }
        const size_t old_group = groups[best_j], new_group = groups[best_i];
        for (size_t i = 0; i < n; ++i)
            if (groups[i] == old_group) groups[i] = new_group;
    }
    size_t numbering[32];
    for (size_t i = 0; i < n; ++i) numbering[i] = SIZE_MAX;
    size_t next = 0;
    for (size_t i = 0; i < n; ++i) {
        if (numbering[groups[i]] == SIZE_MAX) numbering[groups[i]] = next++;
        labels[i] = numbering[groups[i]];
    }
    return true;
}
