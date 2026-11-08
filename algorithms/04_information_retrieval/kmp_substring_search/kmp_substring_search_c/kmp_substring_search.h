#ifndef EDUJBARRIOS_AI_KMP_SUBSTRING_SEARCH_H
#define EDUJBARRIOS_AI_KMP_SUBSTRING_SEARCH_H

#include <stdbool.h>
#include <stddef.h>

/* Printable ASCII, at most 512 characters per string.
   positions must point to storage for 513 size_t values.
   On failure both outputs are unchanged. */
bool kmp_substring_search(const char *text, const char *pattern,
                          size_t positions[513], size_t *count_out);

#endif
