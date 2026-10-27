#ifndef EDUJBARRIOS_AI_REPETITION_PENALTY_H
#define EDUJBARRIOS_AI_REPETITION_PENALTY_H
#include <stdbool.h>
#include <stddef.h>
/* Vocabulary 1..4096; length <=16384; penalty 1..10.
   Output is a distinct V-element buffer, unchanged on failure. */
bool repetition_penalty(const double *logits, size_t vocab,
                        const size_t *history, size_t length,
                        double penalty, double *out);
#endif
