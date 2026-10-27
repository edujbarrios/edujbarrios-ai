#ifndef EDUJBARRIOS_AI_IMAGE_TEXT_MRR_H
#define EDUJBARRIOS_AI_IMAGE_TEXT_MRR_H
#include <stdbool.h>
#include <stddef.h>
/* Inputs: paired row-major N*D embeddings, N=2..64, D=1..128.
   Nonoverlapping caller-provided result[2] is unchanged on failure. */
bool image_text_mrr(const double *images, const double *texts,
                    size_t n, size_t dim, double result[2]);
#endif
