#ifndef EDUJBARRIOS_AI_ISOTONIC_REGRESSION_H
#define EDUJBARRIOS_AI_ISOTONIC_REGRESSION_H
#include <stdbool.h>
#include <stddef.h>
/* Least-squares nondecreasing fit; 1..512 bounded finite samples.
   Validate before writing; output may alias values. */
bool isotonic_regression(const double *values, size_t n, double *output);
#endif
