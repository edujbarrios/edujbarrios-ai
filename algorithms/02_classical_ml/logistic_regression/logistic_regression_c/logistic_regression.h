#ifndef EDUJBARRIOS_AI_LOGISTIC_REGRESSION_H
#define EDUJBARRIOS_AI_LOGISTIC_REGRESSION_H
#include <stdbool.h>
#include <stddef.h>

/* Train a one-feature binary logistic model by full-batch gradient descent.
   x/y have at least n elements; outputs must be distinct non-null pointers.
   Outputs are not modified if the function returns false. */
bool logistic_regression(const double *x, const double *y, size_t n,
                         double learning_rate, size_t epochs,
                         double *weight_out, double *bias_out);
#endif
