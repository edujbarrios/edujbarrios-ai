#ifndef EDUJBARRIOS_AI_R_SQUARED_SCORE_H
#define EDUJBARRIOS_AI_R_SQUARED_SCORE_H
#include <stdbool.h>
#include <stddef.h>

/* At least two finite paired values; output unchanged on failure. */
bool r_squared_score(const double *actual, const double *predicted,
                     size_t n, double *result);
#endif
