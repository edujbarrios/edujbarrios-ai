#include "contrastive_pair_loss.h"
#include <math.h>

bool contrastive_pair_loss(const double *first, const double *second,
                           size_t d, uint8_t label, double margin, double *output) {
    if (!first || !second || !output || d == 0 || d > 512 || label > 1 ||
        !isfinite(margin) || margin < 0 || margin > 1000) return false;
    double squared = 0.0;
    for (size_t i = 0; i < d; ++i) {
        if (!isfinite(first[i]) || !isfinite(second[i]) ||
            fabs(first[i]) > 1000 || fabs(second[i]) > 1000) return false;
        const double difference = first[i] - second[i];
        squared += difference * difference;
    }
    const double gap = fmax(0.0, margin - sqrt(squared));
    *output = 0.5 * (label == 1 ? squared : gap * gap);
    return true;
}
