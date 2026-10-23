#include "top_k.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct scored_item {
    double score;
    size_t index;
};
static int rank_compare(const void *pa, const void *pb) {
    const struct scored_item *a = pa;
    const struct scored_item *b = pb;
    if (a->score > b->score) return -1;
    if (a->score < b->score) return 1;
    if (a->index < b->index) return -1;
    if (a->index > b->index) return 1;
    return 0;
}
bool top_k(const double *scores, size_t n, size_t k, size_t *indices) {
    if (!scores || !indices || n == 0 || k == 0 || k > n ||
        n > SIZE_MAX / sizeof(struct scored_item)) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(scores[i])) return false;
    }
    struct scored_item *items = malloc(n * sizeof(*items));
    if (!items) return false;
    for (size_t i = 0; i < n; ++i) {
        items[i].score = scores[i];
        items[i].index = i;
    }
    qsort(items, n, sizeof(*items), rank_compare);
    for (size_t i = 0; i < k; ++i) indices[i] = items[i].index;
    free(items);
    return true;
}
