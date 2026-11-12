#include "triplet_margin_loss.h"
#include <math.h>

bool triplet_margin_loss(const double *anchor, const double *positive,
                         const double *negative, size_t d, double margin,
                         double *output) {
    if (!anchor || !positive || !negative || !output || d == 0 || d > 512 ||
        !isfinite(margin) || margin < 0.0 || margin > 1000.0) return false;
    double pos_sq = 0.0, neg_sq = 0.0;
    for (size_t i = 0; i < d; ++i) {
        if (!isfinite(anchor[i]) || !isfinite(positive[i]) ||
            !isfinite(negative[i]) || fabs(anchor[i]) > 1000000.0 ||
            fabs(positive[i]) > 1000000.0 ||
            fabs(negative[i]) > 1000000.0) return false;
        const double dp = anchor[i] - positive[i];
        const double dn = anchor[i] - negative[i];
        pos_sq += dp * dp;
        neg_sq += dn * dn;
    }
    const double difference = sqrt(pos_sq) - sqrt(neg_sq) + margin;
    *output = fmax(0.0, difference);
    return true;
}
