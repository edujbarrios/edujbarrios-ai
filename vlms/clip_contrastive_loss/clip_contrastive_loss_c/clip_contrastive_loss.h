#ifndef EDUJBARRIOS_AI_CLIP_CONTRASTIVE_LOSS_H
#define EDUJBARRIOS_AI_CLIP_CONTRASTIVE_LOSS_H
#include <stdbool.h>
#include <stddef.h>
/* Matched row-major N by D embeddings. N 2..64; D 1..128.
   The output must not overlap inputs and remains unchanged on failure. */
bool clip_contrastive_loss(const double *images, const double *texts,
                           size_t n, size_t dim, double logit_scale,
                           double *loss_out);
#endif
