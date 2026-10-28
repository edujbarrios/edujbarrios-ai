#ifndef EDUJBARRIOS_VLM_CROSS_MODAL_ATTENTION_H
#define EDUJBARRIOS_VLM_CROSS_MODAL_ATTENTION_H
#include <stdbool.h>
#include <stddef.h>
/* P=1..64, D=1..64. Input keys/values are flat P*D row-major arrays.
   Outputs weights[P], context[D] must be distinct from inputs and each other,
   and remain unchanged on input validation failure. */
bool cross_modal_attention(const double *query, const double *keys,
                           const double *values, size_t patches, size_t dim,
                           double *weights, double *context);
#endif
