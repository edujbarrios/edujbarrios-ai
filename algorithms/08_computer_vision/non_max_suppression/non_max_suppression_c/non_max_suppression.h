#ifndef EDUJBARRIOS_AI_NON_MAX_SUPPRESSION_H
#define EDUJBARRIOS_AI_NON_MAX_SUPPRESSION_H
#include <stdbool.h>
#include <stddef.h>
typedef struct {
    double x1, y1, x2, y2, score;
} NMSBox;
/* 0..256 boxes. On error, indices and *selected_count remain unchanged.
   If count>0, indices needs count entries. Buffers must not overlap. */
bool non_max_suppression(const NMSBox *boxes, size_t count, double threshold,
                         size_t *indices, size_t *selected_count);
#endif
