#ifndef EDUJBARRIOS_AI_GAUSSIAN_BLUR_H
#define EDUJBARRIOS_AI_GAUSSIAN_BLUR_H
#include <stdbool.h>
#include <stddef.h>

/* Flat grayscale image, dimensions 1..64, finite pixels in +/-1000000.
   out has height*width entries, must not alias pixels; unchanged on error. */
bool gaussian_blur(const double *pixels, size_t height, size_t width, double *out);

#endif
