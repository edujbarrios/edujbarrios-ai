#ifndef GRADIENT_CLIP_GLOBAL_NORM_H
#define GRADIENT_CLIP_GLOBAL_NORM_H
#include <stdbool.h>
#include <stddef.h>
/* On failure output is unchanged; exact in-place clipping is supported. */
bool gradient_clip_global_norm(const double *gradients, size_t n, double max_norm, double *output);
#endif
