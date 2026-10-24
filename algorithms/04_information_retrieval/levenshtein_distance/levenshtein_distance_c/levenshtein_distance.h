#ifndef EDUJBARRIOS_AI_LEVENSHTEIN_DISTANCE_H
#define EDUJBARRIOS_AI_LEVENSHTEIN_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>
/* Printable ASCII NUL-terminated strings of at most 512 bytes.
   On failure distance_out is unchanged. */
bool levenshtein_distance(const char *a,const char *b,size_t *distance_out);
#endif
