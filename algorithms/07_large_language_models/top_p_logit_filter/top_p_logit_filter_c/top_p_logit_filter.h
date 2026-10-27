#ifndef EDUJBARRIOS_AI_TOP_P_LOGIT_FILTER_H
#define EDUJBARRIOS_AI_TOP_P_LOGIT_FILTER_H
#include <stdbool.h>
#include <stddef.h>
/* Valid V=1..4096, logits finite in [-1000,1000], 0<top_p<=1.
   The output contains V booleans and may not overlap logits.
   Output remains untouched on invalid input. */
bool top_p_logit_filter(const double *logits, size_t vocab, double top_p,
                        bool *keep);
#endif
