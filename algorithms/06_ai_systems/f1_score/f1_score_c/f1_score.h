#ifndef EDUJBARRIOS_AI_F1_SCORE_H
#define EDUJBARRIOS_AI_F1_SCORE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* actual/predicted are n binary labels; n>0. Caller guarantees capacities.
   Output pointer must not alias either input; unchanged on error. */
bool f1_score(const uint8_t *actual, const uint8_t *predicted,
              size_t n, double *score);
#endif
