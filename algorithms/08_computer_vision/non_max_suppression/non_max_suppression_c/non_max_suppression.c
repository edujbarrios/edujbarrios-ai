#include "non_max_suppression.h"
#include <math.h>

static double overlap(const NMSBox *a, const NMSBox *b) {
    double w = fmax(0.0, fmin(a->x2, b->x2) - fmax(a->x1, b->x1));
    double h = fmax(0.0, fmin(a->y2, b->y2) - fmax(a->y1, b->y1));
    double intersection = w * h;
    double aa = (a->x2 - a->x1) * (a->y2 - a->y1);
    double ab = (b->x2 - b->x1) * (b->y2 - b->y1);
    return intersection / (aa + ab - intersection);
}

bool non_max_suppression(const NMSBox *boxes, size_t count, double threshold,
                         size_t *indices, size_t *selected_count) {
    if (!selected_count || count > 256 || (count && (!boxes || !indices)) ||
        !isfinite(threshold) || threshold < 0.0 || threshold > 1.0)
        return false;
    for (size_t i = 0; i < count; ++i) {
        const NMSBox b = boxes[i];
        if (!isfinite(b.x1) || !isfinite(b.y1) ||
            !isfinite(b.x2) || !isfinite(b.y2) || !isfinite(b.score) ||
            fabs(b.x1) > 1000000.0 || fabs(b.y1) > 1000000.0 ||
            fabs(b.x2) > 1000000.0 || fabs(b.y2) > 1000000.0 ||
            b.x2 <= b.x1 || b.y2 <= b.y1 ||
            b.score < 0.0 || b.score > 1.0) return false;
    }
    bool discarded[256] = {false};
    size_t result[256], kept = 0;
    for (size_t step = 0; step < count; ++step) {
        size_t best = count;
        for (size_t i = 0; i < count; ++i) {
            if (!discarded[i] &&
                (best == count || boxes[i].score > boxes[best].score))
                best = i;
        }
        if (best == count) break;
        result[kept++] = best;
        discarded[best] = true;
        for (size_t i = 0; i < count; ++i)
            if (!discarded[i] && overlap(&boxes[best], &boxes[i]) > threshold)
                discarded[i] = true;
    }
    for (size_t i = 0; i < kept; ++i) indices[i] = result[i];
    *selected_count = kept;
    return true;
}
