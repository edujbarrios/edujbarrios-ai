#ifndef EDUJBARRIOS_AI_SYMMETRIC_MAPE_H
#define EDUJBARRIOS_AI_SYMMETRIC_MAPE_H
#include <stdbool.h>
#include <stddef.h>

/* Mean 2*abs(pred-target)/(abs(pred)+abs(target)); zero/zero -> 0.
   Input n in 1..10000, finite bounds +/-1e6.
   Caller-owned output unchanged on error and must not overlap inputs. */
bool symmetric_mean_absolute_percentage_error(
    const double *target, const double *prediction, size_t n, double *out);
#endif
