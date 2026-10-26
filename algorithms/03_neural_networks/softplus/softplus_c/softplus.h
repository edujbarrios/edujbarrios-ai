#ifndef EDUJBARRIOS_AI_SOFTPLUS_H
#define EDUJBARRIOS_AI_SOFTPLUS_H

#include <stdbool.h>
#include <stddef.h>

/* Distinct arrays with n elements; no outputs changed on invalid input. */
bool softplus(const double *inputs, size_t n, double *values, double *derivatives);

#endif
