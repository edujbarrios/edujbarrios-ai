#ifndef EDUJBARRIOS_AI_TEMPERATURE_SOFTMAX_H
#define EDUJBARRIOS_AI_TEMPERATURE_SOFTMAX_H
#include <stdbool.h>
#include <stddef.h>
/* Finite logits[n], 1<=n<=4096, temperature>0 and finite.
   Output probabilities[n] must not alias input and stays unchanged on error. */
bool temperature_softmax(const double *logits,size_t n,double temperature,
                         double *probabilities);
#endif
