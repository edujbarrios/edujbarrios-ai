#ifndef EDUJBARRIOS_AI_LAYER_NORMALIZATION_H
#define EDUJBARRIOS_AI_LAYER_NORMALIZATION_H
#include <stdbool.h>
#include <stddef.h>
/* Input, gain, shift and output arrays have n elements; output disjoint
   from all inputs. On error output is unchanged. epsilon > 0. */
bool layer_normalization(const double *input, const double *gamma,
                         const double *beta, size_t n, double epsilon,
                         double *output);
#endif
