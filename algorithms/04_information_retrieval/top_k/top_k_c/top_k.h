#ifndef EDUJBARRIOS_AI_TOP_K_H
#define EDUJBARRIOS_AI_TOP_K_H
#include <stdbool.h>
#include <stddef.h>
/* Caller supplies n finite double scores and output with k slots.
   Returns false without writing output when input is invalid. */
bool top_k(const double *scores, size_t n, size_t k, size_t *indices);
#endif
