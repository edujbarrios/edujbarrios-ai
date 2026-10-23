#ifndef EDUJBARRIOS_AI_SCALAR_QUANTIZATION_H
#define EDUJBARRIOS_AI_SCALAR_QUANTIZATION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Quantize n double values to bytes. Caller guarantees at least n elements
   in both input/output buffers. Input may be NULL if n == 0. Output must
   be non-null. Output remains unchanged on failure. */
bool scalar_quantization(const double *input, size_t n,
                         double minimum, double maximum, uint8_t *output);
#endif
