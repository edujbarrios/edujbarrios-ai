#ifndef EDUJBARRIOS_AI_CONTRASTIVE_PAIR_LOSS_H
#define EDUJBARRIOS_AI_CONTRASTIVE_PAIR_LOSS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Label 1 = similar, 0 = dissimilar. Bounds: d 1..512, |value|<=1000,
   finite margin 0..1000. Output unchanged on invalid arguments. */
bool contrastive_pair_loss(const double *first, const double *second,
                           size_t d, uint8_t label, double margin, double *output);
#endif
