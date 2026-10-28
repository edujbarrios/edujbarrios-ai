#ifndef EDUJBARRIOS_CNN_VALID_CONV2D_H
#define EDUJBARRIOS_CNN_VALID_CONV2D_H
#include <stdbool.h>
#include <stddef.h>
/* Separate output[(H-KH+1)*(W-KW+1)] is not modified on failure. */
bool valid_conv2d(const double *image, size_t height, size_t width,
                  const double *kernel, size_t kh, size_t kw, double *out);
#endif
