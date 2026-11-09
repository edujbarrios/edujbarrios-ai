#ifndef EDUJBARRIOS_AI_BOYER_MOORE_HORSPOOL_SEARCH_H
#define EDUJBARRIOS_AI_BOYER_MOORE_HORSPOOL_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
/* Printable ASCII NUL-terminated strings of length <=512.
   Positions requires capacity 513. All outputs unchanged on failure. */
bool boyer_moore_horspool_search(
    const char *text, const char *pattern, size_t positions[513],
    size_t *count_out);
#endif
