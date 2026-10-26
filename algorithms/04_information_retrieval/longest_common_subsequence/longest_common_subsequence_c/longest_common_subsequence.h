#ifndef EDUJBARRIOS_AI_LONGEST_COMMON_SUBSEQUENCE_H
#define EDUJBARRIOS_AI_LONGEST_COMMON_SUBSEQUENCE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Report LCS length; null token pointers permitted only for zero lengths.
   On invalid input or allocation failure, output remains unchanged. */
bool longest_common_subsequence(const int32_t *a, size_t n,
                                const int32_t *b, size_t m, size_t *result);

#endif
