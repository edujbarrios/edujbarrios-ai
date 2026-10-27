#ifndef EDUJBARRIOS_AI_SIGLIP_PAIRWISE_LOSS_H
#define EDUJBARRIOS_AI_SIGLIP_PAIRWISE_LOSS_H
#include <stdbool.h>
#include <stddef.h>
/* Flat row-major image/text embeddings, 2<=N<=64, 1<=D<=128,
   0<logit_scale<=100 and -20<=bias<=20. The result pointer is
   independent of input buffers and unchanged if validation fails. */
bool siglip_pairwise_loss(const double *images, const double *texts,
                          size_t n, size_t dim, double logit_scale,
                          double bias, double *loss);
#endif
