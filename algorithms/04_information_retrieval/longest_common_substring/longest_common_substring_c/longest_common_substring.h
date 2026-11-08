#ifndef EDUJBARRIOS_AI_LONGEST_COMMON_SUBSTRING_H
#define EDUJBARRIOS_AI_LONGEST_COMMON_SUBSTRING_H
#include <stdbool.h>
#include <stddef.h>
/* Printable ASCII NUL-terminated strings, <=512 characters.
   Three output pointers must be non-NULL and mutually distinct.
   On failure all outputs remain unchanged. */
bool longest_common_substring(const char *a, const char *b,
                              size_t *length_out,
                              size_t *start_a_out, size_t *start_b_out);
#endif
