#ifndef EDUJBARRIOS_AI_LONGEST_PALINDROMIC_SUBSTRING_H
#define EDUJBARRIOS_AI_LONGEST_PALINDROMIC_SUBSTRING_H
#include <stdbool.h>
#include <stddef.h>
/* Printable ASCII NUL-terminated string of length 0..512.
   Outputs are distinct non-null pointers and unchanged on failure. */
bool longest_palindromic_substring(const char *text,
                                   size_t *length_out, size_t *start_out);
#endif
