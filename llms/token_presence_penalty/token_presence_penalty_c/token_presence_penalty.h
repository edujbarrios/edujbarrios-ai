#ifndef TOKEN_PRESENCE_PENALTY_H
#define TOKEN_PRESENCE_PENALTY_H
#include <stdbool.h>
#include <stddef.h>
/* Invalid input leaves output unchanged. Exact in-place logits/output allowed. */
bool token_presence_penalty(const double *logits, size_t vocab_size,
                            const size_t *history, size_t history_length,
                            double penalty, double *output);
#endif
