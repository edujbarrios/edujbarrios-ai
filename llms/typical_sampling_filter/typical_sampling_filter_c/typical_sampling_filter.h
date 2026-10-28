#ifndef EDUJBARRIOS_LLM_TYPICAL_SAMPLING_FILTER_H
#define EDUJBARRIOS_LLM_TYPICAL_SAMPLING_FILTER_H
#include <stdbool.h>
#include <stddef.h>
/* V=1..4096, finite scores in [-1000,1000], 0<p<=1.
   Separate caller-owned V-element output, unchanged on invalid input. */
bool typical_sampling_filter(const double *logits, size_t vocab,
                             double p, bool *keep);
#endif
