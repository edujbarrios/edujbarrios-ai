#ifndef EDUJBARRIOS_CNN_GLOBAL_AVERAGE_POOL2D_H
#define EDUJBARRIOS_CNN_GLOBAL_AVERAGE_POOL2D_H
#include <stdbool.h>
#include <stddef.h>
/* CHW input, C=1..16, H/W=1..32. Output[C] must not overlap input.
   On invalid input output is untouched. */
bool global_average_pool2d(const double *input, size_t channels,
                            size_t height, size_t width, double *output);
#endif
