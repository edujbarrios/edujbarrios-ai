#ifndef EDUJBARRIOS_AI_SAME_CONV2D_H
#define EDUJBARRIOS_AI_SAME_CONV2D_H
#include <stdbool.h>
#include <stddef.h>
/* Zero-padded stride-one cross-correlation with same HxW output.
   Image H,W 1..32; odd KH,KW each in {1,3,5}. Finite real inputs
   bounded by 1000 in magnitude. Distinct output H*W doubles,
   left unchanged on validation errors. */
bool same_conv2d(const double *image,size_t height,size_t width,
                 const double *kernel,size_t kh,size_t kw,double *output);
#endif
