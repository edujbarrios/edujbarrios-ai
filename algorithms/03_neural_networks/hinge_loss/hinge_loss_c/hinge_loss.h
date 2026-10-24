#ifndef EDUJBARRIOS_AI_HINGE_LOSS_H
#define EDUJBARRIOS_AI_HINGE_LOSS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Nonempty scores[n], signed labels[n] in {-1,+1}.
   Caller owns loss scalar and gradients[n], nonoverlapping.
   Both outputs unchanged on false. At margin == 1, gradient == 0. */
bool hinge_loss(const double *scores,const int8_t *labels,size_t n,
                double *mean_loss,double *gradients);
#endif
