#ifndef EDUJBARRIOS_CNN_MAX_POOL2D_H
#define EDUJBARRIOS_CNN_MAX_POOL2D_H
#include <stdbool.h>
#include <stddef.h>
/* Max-pool a valid HxW float image using 2x2 windows, stride 2.
   Caller-provided output[(H/2)*(W/2)] must not overlap input. */
bool max_pool2d(const double *input, size_t height, size_t width,
                double *output);
#endif
