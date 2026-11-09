#ifndef EDUJBARRIOS_AI_Z_ALGORITHM_SUBSTRING_SEARCH_H
#define EDUJBARRIOS_AI_Z_ALGORITHM_SUBSTRING_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
/* Printable ASCII NUL-terminated strings up to 512 characters each.
   Caller provides positions array of 513 size_t elements and a distinct
   count pointer. Invalid inputs do not change outputs. */
bool z_algorithm_substring_search(
    const char *text, const char *pattern, size_t positions[513],
    size_t *count_out);
#endif
