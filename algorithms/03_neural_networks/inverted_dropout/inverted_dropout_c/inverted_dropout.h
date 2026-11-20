#ifndef EDUJBARRIOS_AI_INVERTED_DROPOUT_H
#define EDUJBARRIOS_AI_INVERTED_DROPOUT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Nonnull pointers, even for n=0. Input/output memory must not overlap.
 * Finite activations, binary mask, finite probability in [0,1).
 * No output is changed if validation or scaling fails. */
bool inverted_dropout(const double *activations, const uint8_t *mask,
                      size_t n, double probability, double *output);
#endif
