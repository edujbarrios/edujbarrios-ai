#ifndef EDUJBARRIOS_AI_ADAMW_OPTIMIZER_H
#define EDUJBARRIOS_AI_ADAMW_OPTIMIZER_H
#include <stdbool.h>
#include <stddef.h>
/* Four aligned input vectors, step 1..1,000,000 and positive learning rate.
   Three disjoint caller-owned output arrays remain untouched on error.
   beta1=.9, beta2=.999, epsilon=1e-8 and decoupled decay=.01. */
bool adamw_optimizer(const double *theta,const double *gradient,
    const double *m,const double *v,size_t n,size_t step,double learning_rate,
    double *theta_next,double *m_next,double *v_next);
#endif
