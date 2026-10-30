#ifndef GRADIENT_CLIP_BY_VALUE_H
#define GRADIENT_CLIP_BY_VALUE_H
#include <stdbool.h>
#include <stddef.h>
/* Exact in-place supported; invalid inputs leave output untouched. */
bool gradient_clip_by_value(const double *gradients, size_t n, double clip_value, double *output);
#endif
