#ifndef EDUJBARRIOS_AI_MIN_P_LOGIT_FILTER_H
#define EDUJBARRIOS_AI_MIN_P_LOGIT_FILTER_H
#include <stdbool.h>
#include <stddef.h>
/* 1..4096 finite logits in [-1000,1000], min_p in [0,1].
   Caller-allocated keep[V] stays unchanged on validation failure.
   Input/output must not overlap. */
bool min_p_logit_filter(const double *logits, size_t vocab,
                        double min_p, bool *keep);
#endif
