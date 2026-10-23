#include "jaccard_similarity.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int cmp_i32(const void *pa, const void *pb) {
    const int32_t a = *(const int32_t *)pa;
    const int32_t b = *(const int32_t *)pb;
    return (a > b) - (a < b);
}
static size_t next_unique(const int32_t *arr, size_t n, size_t i) {
    const int32_t v = arr[i];
    do { ++i; } while (i < n && arr[i] == v);
    return i;
}
bool jaccard_similarity(const int32_t *left, size_t n_left,
                        const int32_t *right, size_t n_right, double *out) {
    if (!out || (n_left && !left) || (n_right && !right) ||
        n_left > SIZE_MAX / sizeof(int32_t) ||
        n_right > SIZE_MAX / sizeof(int32_t) ||
        n_left > SIZE_MAX - n_right) return false;
    if (n_left == 0 && n_right == 0) {
        *out = 1.0;
        return true;
    }
    int32_t *a = NULL, *b = NULL;
    if (n_left) {
        a = malloc(n_left * sizeof(*a));
        if (!a) return false;
        memcpy(a, left, n_left * sizeof(*a));
        qsort(a, n_left, sizeof(*a), cmp_i32);
    }
    if (n_right) {
        b = malloc(n_right * sizeof(*b));
        if (!b) { free(a); return false; }
        memcpy(b, right, n_right * sizeof(*b));
        qsort(b, n_right, sizeof(*b), cmp_i32);
    }
    size_t i = 0, j = 0, intersection = 0, union_count = 0;
    while (i < n_left || j < n_right) {
        if (j >= n_right || (i < n_left && a[i] < b[j])) {
            i = next_unique(a, n_left, i);
        } else if (i >= n_left || b[j] < a[i]) {
            j = next_unique(b, n_right, j);
        } else {
            ++intersection;
            i = next_unique(a, n_left, i);
            j = next_unique(b, n_right, j);
        }
        ++union_count;
    }
    const double result = (double)intersection / (double)union_count;
    free(a);
    free(b);
    *out = result;
    return true;
}
