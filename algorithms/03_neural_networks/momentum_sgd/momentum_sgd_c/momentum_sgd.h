#ifndef EDUJBARRIOS_AI_MOMENTUM_SGD_H
#define EDUJBARRIOS_AI_MOMENTUM_SGD_H
#include <stdbool.h>
#include <stddef.h>
/* theta[n], gradient[n], velocity[n] and caller-owned, non-overlapping
   theta_out[n], velocity_out[n]. Failure leaves both outputs unchanged. */
bool momentum_sgd(const double *theta,const double *gradient,
                  const double *velocity,size_t n,double learning_rate,
                  double *theta_out,double *velocity_out);
#endif
