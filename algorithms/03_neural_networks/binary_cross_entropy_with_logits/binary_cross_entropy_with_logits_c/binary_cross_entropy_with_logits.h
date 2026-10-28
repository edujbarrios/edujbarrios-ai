#ifndef EDUJBARRIOS_AI_BCE_WITH_LOGITS_H
#define EDUJBARRIOS_AI_BCE_WITH_LOGITS_H
#include <stdbool.h>
#include <stddef.h>

/* Mean binary cross-entropy from logits and targets in [0,1].
 * On failure, *out is unchanged; output must not overlap inputs. */
bool binary_cross_entropy_with_logits(const double *logits,
    const double *targets, size_t n, double *out);
#endif
