#ifndef EDUJBARRIOS_AI_AVERAGE_POOL2D_H
#define EDUJBARRIOS_AI_AVERAGE_POOL2D_H
#include <stdbool.h>
#include <stddef.h>
/* Flat image HxW (H,W=2..32), finite values [-1000,1000].
   Caller-owned distinct output capacity floor(H/2)*floor(W/2).
   Invalid inputs leave output unchanged. */
bool average_pool2d(const double *image,size_t height,size_t width,double *output);
#endif
