#ifndef SPARSEMAX_H
#define SPARSEMAX_H

#include <stdbool.h>
#include <stddef.h>

/* Project n finite logits onto the probability simplex. Returns false on
   invalid input or allocation failure; output is unchanged on failure. */
bool sparsemax(const double *logits, size_t n, double *output);

#endif
