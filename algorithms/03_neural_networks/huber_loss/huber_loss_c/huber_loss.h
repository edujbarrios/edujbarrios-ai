#ifndef EDUJBARRIOS_AI_HUBER_LOSS_H
#define EDUJBARRIOS_AI_HUBER_LOSS_H
#include <stdbool.h>
#include <stddef.h>
/* Aligned finite predictions[n], targets[n], finite positive delta.
   Caller owns disjoint loss scalar and gradients[n]; no output
   is modified on error. */
bool huber_loss(const double *predictions,const double *targets,size_t n,
                double delta,double *mean_loss,double *gradients);
#endif
