#ifndef EDUJBARRIOS_AI_RABIN_KARP_SUBSTRING_SEARCH_H
#define EDUJBARRIOS_AI_RABIN_KARP_SUBSTRING_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
/* Printable ASCII strings of at most 512 characters; NUL-terminated.
   positions requires capacity 513; on failure outputs remain unchanged. */
bool rabin_karp_substring_search(const char *text, const char *pattern,
                                 size_t positions[513], size_t *count_out);
#endif
