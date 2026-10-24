#ifndef EDUJBARRIOS_AI_EXPECTED_CALIBRATION_ERROR_H
#define EDUJBARRIOS_AI_EXPECTED_CALIBRATION_ERROR_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Finite p[i] in [0,1], binary labels, 1<=bins<=100.
   Caller-owned output scalar stays unchanged on invalid inputs.
   Input and output buffers must not overlap. */
bool expected_calibration_error(const double *probabilities,
    const uint8_t *labels,size_t n,size_t bins,double *result);
#endif
