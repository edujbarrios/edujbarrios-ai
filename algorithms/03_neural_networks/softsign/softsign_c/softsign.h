#ifndef EDUJBARRIOS_AI_SOFTSIGN_H
#define EDUJBARRIOS_AI_SOFTSIGN_H
#include <stdbool.h>
#include <stddef.h>

/* Softsign of 1..10000 finite inputs within [-1000000,1000000].
 * Output length n, unchanged on invalid input; arrays must not overlap. */
bool softsign(const double *inputs, size_t n, double *output);
#endif
