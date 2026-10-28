#ifndef EDUJBARRIOS_VLM_PATCH_TEXT_MAXSIM_H
#define EDUJBARRIOS_VLM_PATCH_TEXT_MAXSIM_H
#include <stdbool.h>
#include <stddef.h>
/* P,T in 1..32, D in 1..64; flat row-major arrays. Separate
   matches[T] and score must not overlap inputs or each other;
   unchanged on invalid input. All match IDs are zero-based. */
bool patch_text_maxsim(const double *patches, size_t n_patches,
                       const double *tokens, size_t n_tokens, size_t dim,
                       size_t *matches, double *score);
#endif
