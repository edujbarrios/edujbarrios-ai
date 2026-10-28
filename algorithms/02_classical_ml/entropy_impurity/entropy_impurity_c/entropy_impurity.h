#ifndef EDUJBARRIOS_AI_ENTROPY_IMPURITY_H
#define EDUJBARRIOS_AI_ENTROPY_IMPURITY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Shannon entropy (bits) for 1..128 bounded integer class counts.
   All-zero counts rejected. Output unchanged on failure, no aliasing. */
bool entropy_impurity(const uint32_t *counts, size_t n, double *out);
#endif
