#include "precision_recall_curve.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    double score;
    uint8_t label;
} scored_label;

static int by_score_desc(const void *pa, const void *pb) {
    const scored_label *a = pa, *b = pb;
    if (a->score > b->score) return -1;
    if (a->score < b->score) return 1;
    return 0;
}

bool precision_recall_curve(const double *scores, const uint8_t *labels,
                            size_t n, pr_curve_point *output, size_t *count) {
    if (!scores || !labels || !output || !count || n == 0 ||
        n > SIZE_MAX / sizeof(scored_label)) return false;
    size_t positives = 0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(scores[i]) || scores[i] < 0.0 || scores[i] > 1.0 ||
            labels[i] > 1) return false;
        positives += labels[i];
    }
    if (positives == 0) return false;
    scored_label *pairs = malloc(n * sizeof(*pairs));
    if (!pairs) return false;
    for (size_t i = 0; i < n; ++i) {
        pairs[i].score = scores[i];
        pairs[i].label = labels[i];
    }
    qsort(pairs, n, sizeof(*pairs), by_score_desc);
    size_t tp = 0, emitted = 0, end = 0;
    while (end < n) {
        const double score = pairs[end].score;
        do {
            tp += pairs[end].label;
            ++end;
        } while (end < n && pairs[end].score == score);
        output[emitted].threshold = score;
        output[emitted].precision = (double)tp / (double)end;
        output[emitted].recall = (double)tp / (double)positives;
        ++emitted;
    }
    *count = emitted;
    free(pairs);
    return true;
}
