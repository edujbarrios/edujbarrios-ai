#ifndef EDUJBARRIOS_AI_CLIP_ZERO_SHOT_CLASSIFICATION_H
#define EDUJBARRIOS_AI_CLIP_ZERO_SHOT_CLASSIFICATION_H
#include <stdbool.h>
#include <stddef.h>
/* Image[D], texts[K*D] (row-major), 2<=K<=64, 1<=D<=128.
   Probabilities[K] must not alias either input; unchanged on failure. */
bool clip_zero_shot_classification(const double *image, const double *texts,
                                   size_t classes, size_t dim,
                                   double logit_scale, double *probabilities);
#endif
