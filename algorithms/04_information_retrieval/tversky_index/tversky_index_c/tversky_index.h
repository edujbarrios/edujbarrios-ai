#ifndef EDUJBARRIOS_AI_TVERSKY_INDEX_H
#define EDUJBARRIOS_AI_TVERSKY_INDEX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Accepts null arrays only for zero lengths. Result unchanged on failure. */
bool tversky_index(const int32_t *a, size_t n, const int32_t *b, size_t m,
                   double alpha, double beta, double *result);
#endif
