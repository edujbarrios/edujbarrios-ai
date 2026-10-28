#ifndef EDUJBARRIOS_CNN_DEPTHWISE_CONV2D_H
#define EDUJBARRIOS_CNN_DEPTHWISE_CONV2D_H
#include <stdbool.h>
#include <stddef.h>
/* CHW input, CxKHxKW weights, same zero padding, 1<=C<=8,
   1<=H,W<=16, KH/KW in {1,3,5}. Distinct output[C*H*W];
   output remains unchanged on invalid input. */
bool depthwise_conv2d(const double *input, size_t channels,
                      size_t height, size_t width, const double *kernels,
                      size_t kh, size_t kw, double *output);
#endif
