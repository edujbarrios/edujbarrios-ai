#ifndef EDUJBARRIOS_AI_MATTHEWS_CORRELATION_COEFFICIENT_H
#define EDUJBARRIOS_AI_MATTHEWS_CORRELATION_COEFFICIENT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Nonempty aligned binary truth[n], predictions[n]. Caller-owned result
   is disjoint and unchanged on failure. Undefined denominator -> 0. */
bool matthews_correlation_coefficient(const uint8_t *truth,
    const uint8_t *predictions,size_t n,double *result);
#endif
