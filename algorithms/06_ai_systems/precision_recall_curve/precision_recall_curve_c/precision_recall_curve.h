#ifndef EDUJBARRIOS_AI_PRECISION_RECALL_CURVE_H
#define EDUJBARRIOS_AI_PRECISION_RECALL_CURVE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    double threshold;
    double precision;
    double recall;
} pr_curve_point;

/* n>0; finite scores in [0,1], binary labels with at least one positive.
 * Caller provides n nonoverlapping output slots; *count is set on success.
 * On any failure, output and count remain unchanged. */
bool precision_recall_curve(const double *scores, const uint8_t *labels,
                            size_t n, pr_curve_point *output, size_t *count);
#endif
