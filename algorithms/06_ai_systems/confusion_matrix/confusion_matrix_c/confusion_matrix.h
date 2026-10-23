#ifndef EDUJBARRIOS_AI_CONFUSION_MATRIX_H
#define EDUJBARRIOS_AI_CONFUSION_MATRIX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* actual/predicted each have n binary labels; n>=1.
   Caller provides output[4] in (TN,FP,FN,TP) order; unchanged on failure.
   Buffers must not overlap, and sizes are caller obligations. */
bool confusion_matrix(const uint8_t *actual, const uint8_t *predicted,
                      size_t n, size_t output[4]);
#endif
