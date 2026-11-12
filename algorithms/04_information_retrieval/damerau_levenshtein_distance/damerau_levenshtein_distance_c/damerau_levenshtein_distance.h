#ifndef EDUJBARRIOS_AI_DAMERAU_LEVENSHTEIN_DISTANCE_H
#define EDUJBARRIOS_AI_DAMERAU_LEVENSHTEIN_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Full (unrestricted) unit-cost Damerau edit distance for 0..256 byte IDs.
   Returns false without writing output on invalid inputs/allocation failure. */
bool damerau_levenshtein_distance(const uint8_t *a, size_t n,
                                  const uint8_t *b, size_t m, size_t *output);
#endif
