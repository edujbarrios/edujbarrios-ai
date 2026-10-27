#ifndef EDUJBARRIOS_AI_BILINEAR_RESIZE_H
#define EDUJBARRIOS_AI_BILINEAR_RESIZE_H
#include <stdbool.h>
#include <stddef.h>
/* Flat row-major grayscale image; dimensions 1..64 each. Output holds
   out_h*out_w doubles, must not overlap pixels; unchanged on failure. */
bool bilinear_resize(const double *pixels, size_t h, size_t w,
                     size_t out_h, size_t out_w, double *out);
#endif
