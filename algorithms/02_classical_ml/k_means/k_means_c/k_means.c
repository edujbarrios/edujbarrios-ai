#include "k_means.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool k_means(const double *samples, size_t n, size_t d,
             size_t k, size_t max_iterations,
             double *centroids_out, size_t *assignments_out) {
    if (!samples || !centroids_out || !assignments_out || n == 0 || d == 0 ||
        k == 0 || k > n || max_iterations == 0 ||
        n > SIZE_MAX / d || k > SIZE_MAX / d ||
        n * d > SIZE_MAX / sizeof(double) ||
        k * d > SIZE_MAX / sizeof(double) ||
        n > SIZE_MAX / sizeof(size_t) ||
        k > SIZE_MAX / sizeof(size_t)) return false;

    for (size_t i = 0; i < n*d; ++i) {
        if (!isfinite(samples[i])) return false;
    }
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < i; ++j) {
            bool same = true;
            for (size_t col = 0; col < d; ++col) {
                if (samples[i*d+col] != samples[j*d+col]) {
                    same = false;
                    break;
                }
            }
            if (same) return false;
        }
    }

    double *centroids = malloc(k*d*sizeof(*centroids));
    double *sum = calloc(k*d, sizeof(*sum));
    size_t *counts = calloc(k, sizeof(*counts));
    size_t *assignments = malloc(n*sizeof(*assignments));
    if (!centroids || !sum || !counts || !assignments) {
        free(centroids); free(sum); free(counts); free(assignments);
        return false;
    }
    memcpy(centroids, samples, k*d*sizeof(*centroids));
    for (size_t i = 0; i < n; ++i) assignments[i] = SIZE_MAX;

    for (size_t iteration = 0; iteration < max_iterations; ++iteration) {
        bool changed = false;
        memset(sum, 0, k*d*sizeof(*sum));
        memset(counts, 0, k*sizeof(*counts));

        for (size_t i = 0; i < n; ++i) {
            double best_distance = INFINITY;
            size_t best_cluster = 0;
            for (size_t cluster = 0; cluster < k; ++cluster) {
                double distance = 0.0;
                for (size_t col = 0; col < d; ++col) {
                    const double delta = samples[i*d+col] - centroids[cluster*d+col];
                    if (!isfinite(delta)) goto failure;
                    distance = hypot(distance, delta);
                    if (!isfinite(distance)) goto failure;
                }
                if (distance < best_distance) {
                    best_distance = distance;
                    best_cluster = cluster;
                }
            }
            if (assignments[i] != best_cluster) changed = true;
            assignments[i] = best_cluster;
            ++counts[best_cluster];
            for (size_t col = 0; col < d; ++col) {
                const size_t offset = best_cluster*d+col;
                sum[offset] += samples[i*d+col];
                if (!isfinite(sum[offset])) goto failure;
            }
        }
        for (size_t cluster = 0; cluster < k; ++cluster) {
            if (counts[cluster] == 0) continue;
            for (size_t col = 0; col < d; ++col) {
                const size_t offset = cluster*d+col;
                centroids[offset] = sum[offset] / (double)counts[cluster];
                if (!isfinite(centroids[offset])) goto failure;
            }
        }
        if (!changed) break;
    }

    memcpy(centroids_out, centroids, k*d*sizeof(*centroids));
    memcpy(assignments_out, assignments, n*sizeof(*assignments));
    free(centroids); free(sum); free(counts); free(assignments);
    return true;

failure:
    free(centroids); free(sum); free(counts); free(assignments);
    return false;
}
