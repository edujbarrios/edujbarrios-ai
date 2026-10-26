#ifndef EDUJBARRIOS_AI_F_BETA_SCORE_H
#define EDUJBARRIOS_AI_F_BETA_SCORE_H

#include <stdbool.h>
#include <stdint.h>

/* Nonnegative int32 counts, finite positive beta; output unchanged on error. */
bool f_beta_score(int32_t tp, int32_t fp, int32_t fn, double beta, double *result);

#endif
