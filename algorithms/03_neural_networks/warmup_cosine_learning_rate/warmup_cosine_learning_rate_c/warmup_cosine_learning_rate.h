#ifndef WARMUP_COSINE_LEARNING_RATE_H
#define WARMUP_COSINE_LEARNING_RATE_H
#include <stdbool.h>
#include <stddef.h>
/* Writes output only for valid step, horizon and rate parameters. */
bool warmup_cosine_learning_rate(double peak_rate, double minimum_rate,
                                 size_t warmup_steps, size_t total_steps,
                                 size_t step, double *output);
#endif
