#ifndef EDUJBARRIOS_AI_DICE_SIMILARITY_H
#define EDUJBARRIOS_AI_DICE_SIMILARITY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Input pointers may be null for zero-length sets only.
   On failure, *result is unchanged. */
bool dice_similarity(const int32_t *first, size_t n,
                     const int32_t *second, size_t m, double *result);

#endif
