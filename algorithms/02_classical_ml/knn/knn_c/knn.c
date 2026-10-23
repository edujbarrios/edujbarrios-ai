#include "knn.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct neighbor {
    double distance;
    size_t index;
};

static int compare_neighbors(const void *a, const void *b) {
    const struct neighbor *left = a;
    const struct neighbor *right = b;
    if (left->distance < right->distance) return -1;
    if (left->distance > right->distance) return 1;
    if (left->index < right->index) return -1;
    if (left->index > right->index) return 1;
    return 0;
}

bool knn(const double *samples, size_t n_samples, size_t dimensions,
         const double *query, size_t k, size_t *results) {
    if (!samples || !query || !results || !n_samples || !dimensions ||
        !k || k > n_samples || n_samples > SIZE_MAX / dimensions ||
        n_samples > SIZE_MAX / sizeof(struct neighbor)) {
        return false;
    }

    for (size_t j = 0; j < dimensions; ++j) {
        if (!isfinite(query[j])) return false;
    }
    struct neighbor *neighbors = malloc(n_samples * sizeof(*neighbors));
    if (!neighbors) return false;

    for (size_t i = 0; i < n_samples; ++i) {
        double distance = 0.0;
        for (size_t j = 0; j < dimensions; ++j) {
            const double x = samples[i * dimensions + j];
            if (!isfinite(x)) {
                free(neighbors);
                return false;
            }
            const double delta = x - query[j];
            if (!isfinite(delta)) {
                free(neighbors);
                return false;
            }
            distance = hypot(distance, delta);
            if (!isfinite(distance)) {
                free(neighbors);
                return false;
            }
        }
        neighbors[i].distance = distance;
        neighbors[i].index = i;
    }
    qsort(neighbors, n_samples, sizeof(*neighbors), compare_neighbors);
    for (size_t i = 0; i < k; ++i) results[i] = neighbors[i].index;
    free(neighbors);
    return true;
}
