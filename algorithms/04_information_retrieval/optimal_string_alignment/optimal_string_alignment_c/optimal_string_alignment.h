#ifndef EDUJBARRIOS_AI_OPTIMAL_STRING_ALIGNMENT_H
#define EDUJBARRIOS_AI_OPTIMAL_STRING_ALIGNMENT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Restricted Damerau edit distance. Null inputs permitted for length 0.
   Leaves output unchanged on failure or allocation error. */
bool optimal_string_alignment(const int32_t *a, size_t n,
                              const int32_t *b, size_t m, size_t *result);
#endif
