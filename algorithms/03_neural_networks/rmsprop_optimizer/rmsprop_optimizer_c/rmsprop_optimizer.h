#ifndef EDUJBARRIOS_AI_RMSPROP_OPTIMIZER_H
#define EDUJBARRIOS_AI_RMSPROP_OPTIMIZER_H
#include <stdbool.h>
#include <stddef.h>
/* Fixed decay=.9 and epsilon=1e-8, lr positive. Input/output vectors
   have capacity n and do not alias each other. On failure, caller-owned
   outputs remain unmodified. */
bool rmsprop_optimizer(const double *parameters,const double *gradients,
    const double *previous_average,size_t n,double learning_rate,
    double *updated_parameters,double *updated_average);
#endif
