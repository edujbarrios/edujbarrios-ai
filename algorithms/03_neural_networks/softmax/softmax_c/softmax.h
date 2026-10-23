#ifndef EDUJBARRIOS_AI_SOFTMAX_H
#define EDUJBARRIOS_AI_SOFTMAX_H
#include <stdbool.h>
#include <stddef.h>

/* Input and output contain at least n doubles, n > 0.
   Exact in-place output supported; output unchanged on invalid input. */
bool softmax(const double *logits, size_t n, double *output);
#endif
