#ifndef EDUJBARRIOS_AI_OVERLAP_COEFFICIENT_H
#define EDUJBARRIOS_AI_OVERLAP_COEFFICIENT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Null input allowed only with zero length; *result unchanged on failure. */
bool overlap_coefficient(const int32_t *a, size_t n, const int32_t *b,
                         size_t m, double *result);
#endif
