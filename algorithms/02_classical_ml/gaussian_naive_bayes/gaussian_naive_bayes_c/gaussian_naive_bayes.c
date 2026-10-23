#include "gaussian_naive_bayes.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static bool valid(double x) { return isfinite(x) != 0; }

bool gaussian_naive_bayes(const double *samples, const uint8_t *labels,
                          size_t n, size_t d, const double *query,
                          uint8_t *predicted, double *probability_1) {
    if (!samples || !labels || !query || !predicted || !probability_1 ||
        n < 2 || d == 0 || n > SIZE_MAX / d ||
        d > SIZE_MAX / (4 * sizeof(double))) return false;
    for (size_t j = 0; j < d; ++j) if (!valid(query[j])) return false;
    size_t count[2] = {0, 0};
    double *mean = calloc(2*d, sizeof(*mean));
    double *var = calloc(2*d, sizeof(*var));
    if (!mean || !var) { free(mean); free(var); return false; }
    for (size_t i = 0; i < n; ++i) {
        if (labels[i] > 1) goto fail;
        const size_t c = labels[i];
        ++count[c];
        for (size_t j = 0; j < d; ++j) {
            const double x = samples[i*d + j];
            if (!valid(x)) goto fail;
            const size_t offset = c*d + j;
            const double delta = x - mean[offset];
            if (!valid(delta)) goto fail;
            mean[offset] += delta / (double)count[c];
            if (!valid(mean[offset])) goto fail;
        }
    }
    if (!count[0] || !count[1]) goto fail;
    for (size_t i = 0; i < n; ++i) {
        const size_t c = labels[i];
        for (size_t j = 0; j < d; ++j) {
            const size_t offset = c*d + j;
            const double delta = samples[i*d+j] - mean[offset];
            const double square = delta * delta;
            if (!valid(delta) || !valid(square)) goto fail;
            var[offset] += square;
            if (!valid(var[offset])) goto fail;
        }
    }
    double score[2] = {0, 0};
    const double two_pi = 6.28318530717958647693;
    for (size_t c = 0; c < 2; ++c) {
        score[c] = log((double)count[c] / (double)n);
        if (!valid(score[c])) goto fail;
        for (size_t j = 0; j < d; ++j) {
            const size_t offset = c*d + j;
            const double variance = fmax(var[offset] / (double)count[c], 1e-9);
            const double delta = query[j] - mean[offset];
            const double square = delta * delta;
            const double term = log(two_pi * variance) + square / variance;
            if (!valid(variance) || !valid(delta) || !valid(square) ||
                !valid(term)) goto fail;
            score[c] -= 0.5 * term;
            if (!valid(score[c])) goto fail;
        }
    }
    const double log_odds = score[1] - score[0];
    if (!valid(log_odds)) goto fail;
    const double e = exp(-fabs(log_odds));
    const double p1 = log_odds >= 0 ? 1.0 / (1.0 + e) : e / (1.0 + e);
    if (!valid(p1)) goto fail;
    *predicted = (uint8_t)(score[1] > score[0] ? 1 : 0);
    *probability_1 = p1;
    free(mean); free(var);
    return true;
fail:
    free(mean); free(var);
    return false;
}
