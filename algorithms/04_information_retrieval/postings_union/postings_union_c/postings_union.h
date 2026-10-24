#ifndef EDUJBARRIOS_AI_POSTINGS_UNION_H
#define EDUJBARRIOS_AI_POSTINGS_UNION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Inputs strictly ascending, unique, IDs <=INT32_MAX. output has at least
   n_left+n_right entries when nonempty. out_count required, unchanged
   on failure, and output must not overlap inputs. */
bool postings_union(const uint32_t *left, size_t n_left,
                    const uint32_t *right, size_t n_right,
                    uint32_t *output, size_t *out_count);
#endif
