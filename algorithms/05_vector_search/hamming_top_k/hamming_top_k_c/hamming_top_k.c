#include "hamming_top_k.h"
#include <stdint.h>
#include <stdlib.h>

#define MAX_SIGNATURE ((UINT32_C(1) << 30) - UINT32_C(1))

struct item { unsigned distance; size_t index; };
static unsigned count_bits(uint32_t bits) {
    unsigned count = 0;
    while (bits != 0) {
        count += (unsigned)(bits & UINT32_C(1));
        bits >>= 1;
    }
    return count;
}
static int compare_items(const void *pa, const void *pb) {
    const struct item *a = pa;
    const struct item *b = pb;
    if (a->distance < b->distance) return -1;
    if (a->distance > b->distance) return 1;
    if (a->index < b->index) return -1;
    if (a->index > b->index) return 1;
    return 0;
}
bool hamming_top_k(uint32_t query, const uint32_t *candidates,
                   size_t n, size_t k, size_t *indices) {
    if (!candidates || !indices || n == 0 || k == 0 || k > n ||
        query > MAX_SIGNATURE || n > SIZE_MAX / sizeof(struct item))
        return false;
    for (size_t i = 0; i < n; ++i)
        if (candidates[i] > MAX_SIGNATURE) return false;
    struct item *items = malloc(n * sizeof(*items));
    if (!items) return false;
    for (size_t i = 0; i < n; ++i) {
        items[i].distance = count_bits(query ^ candidates[i]);
        items[i].index = i;
    }
    qsort(items, n, sizeof(*items), compare_items);
    for (size_t i = 0; i < k; ++i) indices[i] = items[i].index;
    free(items);
    return true;
}
