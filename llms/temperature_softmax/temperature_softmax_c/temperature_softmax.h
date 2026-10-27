#ifndef EDUJBARRIOS_AI_TEMPERATURE_SOFTMAX_H
#define EDUJBARRIOS_AI_TEMPERATURE_SOFTMAX_H
#include <stdbool.h>
#include <stddef.h>
/* 1 <= length <= 4096, finite logits in [-1000,1000], 0.01<=T<=100.
   Output is a separate length-element buffer, untouched on error. */
bool temperature_softmax(const double *logits, size_t length,
                         double temperature, double *probabilities);
#endif
