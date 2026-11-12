#ifndef EDUJBARRIOS_AI_BERNOULLI_NAIVE_BAYES_H
#define EDUJBARRIOS_AI_BERNOULLI_NAIVE_BAYES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Fits two-class Laplace-smoothed Bernoulli NB and predicts one binary query.
   Flat row-major training samples. Outputs unchanged on validation failure. */
bool bernoulli_naive_bayes(const uint8_t *samples, const uint8_t *labels,
                           size_t n, size_t d, const uint8_t *query,
                           uint8_t *predicted_label, double *probability_1);
#endif
