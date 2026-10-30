#include "binary_dice_coefficient.h"
bool binary_dice_coefficient(const uint8_t *prediction, size_t n_prediction,
                             const uint8_t *target, size_t n_target, double *score) {
    if (!prediction || !target || !score || n_prediction == 0 ||
        n_prediction != n_target) return false;
    size_t intersection = 0, count_a = 0, count_b = 0;
    for (size_t i = 0; i < n_prediction; ++i) {
        uint8_t a = prediction[i], b = target[i];
        if (a > 1 || b > 1) return false;
        if (a == 1) ++count_a;
        if (b == 1) ++count_b;
        if (a == 1 && b == 1) ++intersection;
    }
    /* Division before multiplication avoids overflow in 2 * intersection. */
    *score = count_a + count_b == 0 ? 1.0 :
             (2.0 * (double)intersection) / ((double)count_a + (double)count_b);
    return true;
}
