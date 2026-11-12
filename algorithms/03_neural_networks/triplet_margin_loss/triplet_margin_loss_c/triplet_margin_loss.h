#ifndef EDUJBARRIOS_AI_TRIPLET_MARGIN_LOSS_H
#define EDUJBARRIOS_AI_TRIPLET_MARGIN_LOSS_H
#include <stdbool.h>
#include <stddef.h>
/* Euclidean triplet margin loss for 1..512 bounded coordinates.
   False on invalid arguments, preserving caller output. */
bool triplet_margin_loss(const double *anchor, const double *positive,
                         const double *negative, size_t d, double margin,
                         double *output);
#endif
