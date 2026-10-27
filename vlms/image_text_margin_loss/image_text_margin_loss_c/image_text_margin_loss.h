#ifndef EDUJBARRIOS_AI_IMAGE_TEXT_MARGIN_LOSS_H
#define EDUJBARRIOS_AI_IMAGE_TEXT_MARGIN_LOSS_H
#include <stdbool.h>
#include <stddef.h>
/* Input arrays are N*D, N=2..64, D=1..128, finite in [-1000,1000].
   margin in [0,2]. Caller owns distinct result, unchanged on error. */
bool image_text_margin_loss(const double *images, const double *texts,
                            size_t n, size_t dim, double margin,
                            double *result);
#endif
