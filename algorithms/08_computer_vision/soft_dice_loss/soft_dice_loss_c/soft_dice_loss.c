#include "soft_dice_loss.h"
#include <math.h>

bool soft_dice_loss(const double *predictions, const uint8_t *targets,
                    size_t n, double *output) {
    if (!predictions || !targets || !output || n == 0 || n > 512)
        return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(predictions[i]) || predictions[i] < 0 ||
            predictions[i] > 1 || targets[i] > 1) return false;
    }
    double overlap = 0.0, total_p = 0.0, total_t = 0.0;
    for (size_t i = 0; i < n; ++i) {
        overlap += predictions[i] * (double)targets[i];
        total_p += predictions[i];
        total_t += (double)targets[i];
    }
    *output = 1.0 - (2.0 * overlap + 1.0) / (total_p + total_t + 1.0);
    return true;
}
