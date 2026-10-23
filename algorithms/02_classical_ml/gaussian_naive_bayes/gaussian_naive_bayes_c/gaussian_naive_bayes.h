#ifndef EDUJBARRIOS_AI_GAUSSIAN_NAIVE_BAYES_H
#define EDUJBARRIOS_AI_GAUSSIAN_NAIVE_BAYES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Data: samples[n*d] row-major, labels[n] binary, query[d].
   Caller guarantees buffer capacities and distinct output pointers.
   *predicted and *probability_1 unchanged on failure. */
bool gaussian_naive_bayes(const double *samples, const uint8_t *labels,
                          size_t n, size_t d, const double *query,
                          uint8_t *predicted, double *probability_1);
#endif
