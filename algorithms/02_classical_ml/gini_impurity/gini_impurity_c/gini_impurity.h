#ifndef EDUJBARRIOS_AI_GINI_IMPURITY_H
#define EDUJBARRIOS_AI_GINI_IMPURITY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Gini impurity of 1..128 integer class counts in [0,1000000].
   All-zero counts are invalid; output remains unchanged on failure.
   Output must not overlap input counts. */
bool gini_impurity(const uint32_t *counts, size_t n, double *out);
#endif
