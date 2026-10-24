#ifndef EDUJBARRIOS_AI_MULTINOMIAL_NAIVE_BAYES_H
#define EDUJBARRIOS_AI_MULTINOMIAL_NAIVE_BAYES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* n*d row-major counts <=1,000,000, binary labels, both classes present.
   log_priors[2] and log_likelihoods[2*d] remain unchanged on error.
   Caller guarantees adequate, disjoint buffers. */
bool multinomial_naive_bayes(const uint32_t *counts,const uint8_t *labels,
    size_t n,size_t d,double *log_priors,double *log_likelihoods);
/* Prediction returns 0 on exactly tied scores, and preserves output on error. */
bool multinomial_naive_bayes_predict(const double *log_priors,
    const double *log_likelihoods,const uint32_t *query,
    size_t d,uint8_t *prediction);
#endif
