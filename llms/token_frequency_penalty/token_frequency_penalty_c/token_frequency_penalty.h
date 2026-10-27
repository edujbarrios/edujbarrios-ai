#ifndef EDUJBARRIOS_AI_TOKEN_FREQUENCY_PENALTY_H
#define EDUJBARRIOS_AI_TOKEN_FREQUENCY_PENALTY_H
#include <stdbool.h>
#include <stddef.h>
/* 1<=V<=4096, 0<=H<=4096, 0<=a<=5. History token IDs are
   0-based. Output[V] may not overlap input and is unchanged on error. */
bool token_frequency_penalty(const double *logits, size_t vocab,
                             const size_t *history, size_t history_len,
                             double penalty, double *output);
#endif
