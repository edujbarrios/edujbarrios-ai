#ifndef EDUJBARRIOS_AI_KULLBACK_LEIBLER_DIVERGENCE_H
#define EDUJBARRIOS_AI_KULLBACK_LEIBLER_DIVERGENCE_H
#include <stdbool.h>
#include <stddef.h>
/* Both finite categorical vectors, length 1..4096 and normalized ±1e-9.
   Missing Q support for positive P is valid: store +INFINITY and
   return true. All other invalid inputs leave caller-owned result
   unchanged. P, Q and result must not overlap. */
bool kullback_leibler_divergence(const double *p,const double *q,
                                size_t n,double *result);
#endif
