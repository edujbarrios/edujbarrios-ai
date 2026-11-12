#include "bernoulli_naive_bayes.h"
#include <math.h>

bool bernoulli_naive_bayes(const uint8_t *samples, const uint8_t *labels,
                           size_t n, size_t d, const uint8_t *query,
                           uint8_t *predicted_label, double *probability_1) {
    if (!samples || !labels || !query || !predicted_label || !probability_1 ||
        n < 2 || n > 128 || d == 0 || d > 16) return false;

    size_t counts[2] = {0, 0};
    size_t ones[2][16] = {{0}};
    for (size_t i = 0; i < n; ++i) {
        const uint8_t label = labels[i];
        if (label > 1) return false;
        ++counts[label];
        for (size_t j = 0; j < d; ++j) {
            const uint8_t x = samples[i * d + j];
            if (x > 1) return false;
            ones[label][j] += x;
        }
    }
    if (counts[0] == 0 || counts[1] == 0) return false;
    for (size_t j = 0; j < d; ++j) if (query[j] > 1) return false;

    double score[2] = {log((double)counts[0] / (double)n),
                       log((double)counts[1] / (double)n)};
    for (size_t c = 0; c < 2; ++c) {
        for (size_t j = 0; j < d; ++j) {
            const double p = ((double)ones[c][j] + 1.0) / ((double)counts[c] + 2.0);
            score[c] += query[j] ? log(p) : log1p(-p);
        }
    }
    const double delta = score[0] - score[1];
    const double posterior = delta >= 0.0
        ? exp(-delta) / (1.0 + exp(-delta))
        : 1.0 / (1.0 + exp(delta));
    *predicted_label = score[1] > score[0] ? 1 : 0;
    *probability_1 = posterior;
    return true;
}
