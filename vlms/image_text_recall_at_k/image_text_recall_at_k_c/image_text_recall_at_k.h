#ifndef EDUJBARRIOS_AI_IMAGE_TEXT_RECALL_AT_K_H
#define EDUJBARRIOS_AI_IMAGE_TEXT_RECALL_AT_K_H
#include <stdbool.h>
#include <stddef.h>
/* Flat matched N*D image/text rows, 2<=N<=64, 1<=D<=128,
   1<=K<=N. Result is [image_to_text,text_to_image] recall.
   Output must not overlap inputs and stays unchanged on failure. */
bool image_text_recall_at_k(const double *images, const double *texts,
                            size_t n, size_t dim, size_t k,
                            double result[2]);
#endif
