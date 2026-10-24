#include "decision_stump.h"
#include <math.h>

bool decision_stump_predict(double feature, double threshold,
                            uint8_t right_positive, uint8_t *label_out) {
    if (!label_out || !isfinite(feature) || !isfinite(threshold) ||
        right_positive > 1) return false;
    const uint8_t result = (uint8_t)(right_positive ?
        feature > threshold : feature <= threshold);
    *label_out = result;
    return true;
}
bool decision_stump(const double *x, const uint8_t *labels, size_t n,
                    double *threshold_out, uint8_t *right_positive_out,
                    size_t *errors_out) {
    if (!x || !labels || !threshold_out || !right_positive_out ||
        !errors_out || n == 0) return false;
    for (size_t i = 0; i < n; ++i)
        if (!isfinite(x[i]) || labels[i] > 1) return false;

    double best_threshold = x[0];
    uint8_t best_right = 1;
    size_t best_errors = n;
    for (size_t candidate = 0; candidate < n; ++candidate) {
        const double threshold = x[candidate];
        for (uint8_t right = 1; ; --right) {
            size_t errors = 0;
            for (size_t i = 0; i < n; ++i) {
                const uint8_t predicted = (uint8_t)(right ?
                    x[i] > threshold : x[i] <= threshold);
                if (predicted != labels[i]) ++errors;
            }
            if (errors < best_errors ||
                (errors == best_errors &&
                 (threshold < best_threshold ||
                  (threshold == best_threshold && right > best_right)))) {
                best_errors = errors;
                best_threshold = threshold;
                best_right = right;
            }
            if (right == 0) break;
        }
    }
    *threshold_out = best_threshold;
    *right_positive_out = best_right;
    *errors_out = best_errors;
    return true;
}
