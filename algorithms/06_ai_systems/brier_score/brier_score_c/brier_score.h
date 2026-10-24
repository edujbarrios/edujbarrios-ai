#ifndef EDUJBARRIOS_AI_BRIER_SCORE_H
#define EDUJBARRIOS_AI_BRIER_SCORE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Mean squared binary probabilistic error. Arrays have n elements,
   caller-owned result must not overlap inputs and remains unchanged on error. */
bool brier_score(const double *probabilities,const uint8_t *labels,
                 size_t n,double *result);
#endif
