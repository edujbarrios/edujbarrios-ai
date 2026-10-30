#ifndef WEIGHTED_BRIER_SCORE_H
#define WEIGHTED_BRIER_SCORE_H
#include <stdbool.h>
#include <stddef.h>
/* Three arrays of n values. On failure *score remains unchanged. */
bool weighted_brier_score(const double *targets, const double *probabilities,
                          const double *weights, size_t n, double *score);
#endif
