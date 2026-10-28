#ifndef EDUJBARRIOS_AI_SPEARMAN_RANK_CORRELATION_H
#define EDUJBARRIOS_AI_SPEARMAN_RANK_CORRELATION_H
#include <stdbool.h>
#include <stddef.h>

/* Tie-aware Spearman correlation, length 2..128.
 * On failure, *out remains unchanged. Output must not overlap inputs. */
bool spearman_rank_correlation(const double *x, const double *y,
                               size_t n, double *out);
#endif
