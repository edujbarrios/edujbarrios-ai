#ifndef EDUJBARRIOS_AI_IMAGE_TEXT_HARD_NEGATIVES_H
#define EDUJBARRIOS_AI_IMAGE_TEXT_HARD_NEGATIVES_H
#include <stdbool.h>
#include <stddef.h>
/* Flat N*D row-major image and text embeddings (2<=N<=64, 1<=D<=128).
   Caller provides 2*N result slots: first image-to-text, then text-to-image.
   Output may not overlap inputs; on error it remains unchanged. */
bool image_text_hard_negatives(const double *images, const double *texts,
                               size_t n, size_t dim, size_t *indices);
#endif
