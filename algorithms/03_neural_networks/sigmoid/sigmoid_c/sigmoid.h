#ifndef EDUJBARRIOS_AI_SIGMOID_H
#define EDUJBARRIOS_AI_SIGMOID_H
#include <stdbool.h>
#include <stddef.h>

/* Input/output buffers must have at least n doubles. Exact in-place
   updates are allowed. No writes occur on invalid input. */
bool sigmoid(const double *values, size_t n, double *output);
#endif
