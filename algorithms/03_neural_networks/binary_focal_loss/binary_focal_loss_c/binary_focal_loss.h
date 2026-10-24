#ifndef EDUJBARRIOS_AI_BINARY_FOCAL_LOSS_H
#define EDUJBARRIOS_AI_BINARY_FOCAL_LOSS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Nonempty class-one probabilities in [0,1], binary 0/1 labels.
   Fixed alpha=0.25, gamma=2, epsilon=1e-12.
   Caller-owned scalar unchanged on failure, no input/output aliasing. */
bool binary_focal_loss(const double *probabilities,const uint8_t *labels,
                       size_t n,double *result);
#endif
