#include "binary_iou.h"

bool binary_iou(const uint8_t *prediction, size_t prediction_len,
                const uint8_t *target, size_t target_len, double *score) {
    if (!prediction || !target || !score || prediction_len == 0 ||
        prediction_len != target_len) {
        return false;
    }
    size_t intersection = 0;
    size_t union_count = 0;
    for (size_t i = 0; i < prediction_len; ++i) {
        uint8_t a = prediction[i];
        uint8_t b = target[i];
        if (a > 1 || b > 1) return false;
        if (a == 1 && b == 1) ++intersection;
        if (a == 1 || b == 1) ++union_count;
    }
    *score = union_count == 0 ? 1.0 :
             (double)intersection / (double)union_count;
    return true;
}
