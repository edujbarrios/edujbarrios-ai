#include "reciprocal_rank_fusion.h"
#include <stdint.h>
#include <stdlib.h>

struct entry {
    int id;
    double score;
};

static int compare_entries(const void *left, const void *right) {
    const struct entry *a = left;
    const struct entry *b = right;
    if (a->score > b->score) return -1;
    if (a->score < b->score) return 1;
    if (a->id < b->id) return -1;
    if (a->id > b->id) return 1;
    return 0;
}

static bool insert_ranking(const int *ids, size_t n,
                           struct entry *entries, size_t *count) {
    for (size_t rank = 0; rank < n; ++rank) {
        if (ids[rank] < 0) return false;
        /* Each source ranking must not contain duplicates. */
        for (size_t i = 0; i < rank; ++i) {
            if (ids[i] == ids[rank]) return false;
        }
        size_t pos = 0;
        while (pos < *count && entries[pos].id != ids[rank]) ++pos;
        if (pos == *count) {
            entries[pos].id = ids[rank];
            entries[pos].score = 0.0;
            ++*count;
        }
        entries[pos].score += 1.0 / (60.0 + (double)(rank + 1));
    }
    return true;
}

bool reciprocal_rank_fusion(const int *a, size_t len_a,
                            const int *b, size_t len_b,
                            int *out, size_t *out_count) {
    if (!out || !out_count || (len_a && !a) || (len_b && !b) ||
        (len_a == 0 && len_b == 0) ||
        len_a > SIZE_MAX - len_b) return false;
    size_t maximum = len_a + len_b;
    if (maximum > SIZE_MAX / sizeof(struct entry)) return false;
    struct entry *entries = malloc(maximum * sizeof(*entries));
    if (!entries) return false;
    size_t count = 0;
    if (!insert_ranking(a, len_a, entries, &count) ||
        !insert_ranking(b, len_b, entries, &count)) {
        free(entries);
        return false;
    }
    qsort(entries, count, sizeof(*entries), compare_entries);
    for (size_t i = 0; i < count; ++i) out[i] = entries[i].id;
    *out_count = count;
    free(entries);
    return true;
}
