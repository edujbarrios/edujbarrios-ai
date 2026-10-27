#ifndef EDUJBARRIOS_AI_TOP_K_LOGIT_FILTER_H
#define EDUJBARRIOS_AI_TOP_K_LOGIT_FILTER_H
#include <stdbool.h>
#include <stddef.h>
/* 1<=V<=4096, 1<=k<=V. The V-element output may not overlap logits.
   On invalid input, output is unchanged. */
bool top_k_logit_filter(const double *logits, size_t vocab, size_t k,
                        bool *keep);
#endif
