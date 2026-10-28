#ifndef EDUJBARRIOS_AI_LOG_COSH_LOSS_H
#define EDUJBARRIOS_AI_LOG_COSH_LOSS_H
#include <stdbool.h>
#include <stddef.h>

/* Stable mean log(cosh(prediction - target)), n in 1..10000.
   On failure *out remains unchanged; output must not overlap inputs. */
bool log_cosh_loss(const double *target, const double *prediction,
                   size_t n, double *out);
#endif
