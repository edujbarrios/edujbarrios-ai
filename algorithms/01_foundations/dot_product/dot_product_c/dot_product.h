#ifndef EDUJBARRIOS_AI_DOT_PRODUCT_H
#define EDUJBARRIOS_AI_DOT_PRODUCT_H

#include <stdbool.h>
#include <stddef.h>

/* Two finite vectors of n elements. Caller ensures both buffer lengths.
   Returns false on invalid inputs or non-finite result; *out stays unchanged.
   NULL inputs are permitted only for n == 0. */
bool dot_product(const double *left, const double *right, size_t n, double *out);

#endif
