#ifndef EDUJBARRIOS_AI_COSINE_ANNEALING_LEARNING_RATE_H
#define EDUJBARRIOS_AI_COSINE_ANNEALING_LEARNING_RATE_H
#include <stdbool.h>
#include <stddef.h>

/* Stateless cosine schedule, 0 <= step <= total_steps <= 100000.
 * max/min finite in [0,1000000], max>=min.
 * On failure *out is unchanged. */
bool cosine_annealing_learning_rate(double max_rate, double min_rate,
                                    size_t step, size_t total_steps, double *out);
#endif
