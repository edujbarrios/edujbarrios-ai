#ifndef EDUJBARRIOS_AI_MEAN_RECIPROCAL_RANK_H
#define EDUJBARRIOS_AI_MEAN_RECIPROCAL_RANK_H
#include <stdbool.h>
#include <stddef.h>
/* ranks contains n nonnegative first-relevant positions; zero means none.
   Caller guarantees valid input buffers; on failure *out is not changed. */
bool mean_reciprocal_rank(const size_t *ranks, size_t n, double *out);
#endif
