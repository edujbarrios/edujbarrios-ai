#ifndef EDUJBARRIOS_AI_KENDALL_TAU_B_H
#define EDUJBARRIOS_AI_KENDALL_TAU_B_H
#include <stdbool.h>
#include <stddef.h>

/* Tie-adjusted Kendall coefficient for paired vectors of length 2..128.
   On failure, *output is unchanged and must not overlap inputs. */
bool kendall_tau_b(const double *x, const double *y, size_t n,
                   double *output);
#endif
