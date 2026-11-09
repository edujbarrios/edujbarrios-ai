#ifndef EDUJBARRIOS_AI_MACRO_F1_SCORE_H
#define EDUJBARRIOS_AI_MACRO_F1_SCORE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Multiclass unweighted macro-F1, 1..512 labels in [0,classes-1],
   2..16 classes; each absent class contributes 0. Output unchanged
   on invalid arguments. */
bool macro_f1_score(const uint8_t *actual, const uint8_t *predicted,
                    size_t n, size_t classes, double *score_out);
#endif
