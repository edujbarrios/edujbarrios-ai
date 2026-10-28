#ifndef EDUJBARRIOS_AI_TRANSITIVE_CLOSURE_H
#define EDUJBARRIOS_AI_TRANSITIVE_CLOSURE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Row-major binary adjacency input and row-major reflexive reachability output.
 * n is 1..64. Output is unchanged on error and must not overlap input. */
bool transitive_closure(const uint8_t *adjacency, size_t n, uint8_t *output);
#endif
