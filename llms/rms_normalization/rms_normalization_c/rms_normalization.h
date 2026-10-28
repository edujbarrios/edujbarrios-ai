#ifndef EDUJBARRIOS_LLM_RMS_NORMALIZATION_H
#define EDUJBARRIOS_LLM_RMS_NORMALIZATION_H
#include <stdbool.h>
#include <stddef.h>
/* Inputs x[D], gain[D], D=1..1024, epsilon 1e-8..1.
   Caller-owned output[D] is separate and unchanged on invalid input. */
bool rms_normalization(const double *x, const double *gain,
                       size_t dim, double epsilon, double *out);
#endif
