#ifndef EDUJBARRIOS_AI_PEARSON_CORRELATION_H
#define EDUJBARRIOS_AI_PEARSON_CORRELATION_H
#include <stdbool.h>
#include <stddef.h>

/* Correlate paired finite values with n in [2,10000].
   On failure, output remains unchanged; arrays must not overlap output. */
bool pearson_correlation(const double *x, const double *y, size_t n,
                         double *out);
#endif
