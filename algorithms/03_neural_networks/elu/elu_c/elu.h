#ifndef EDUJBARRIOS_AI_ELU_H
#define EDUJBARRIOS_AI_ELU_H
#include <stdbool.h>
#include <stddef.h>

/* ELU(alpha=1) on finite input in [-1e6,1e6], length 1..10000.
   On error, output is unchanged; arrays must not overlap. */
bool elu(const double *input, size_t n, double *output);
#endif
