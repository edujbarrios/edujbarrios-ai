#ifndef EDUJBARRIOS_AI_STRATIFIED_K_FOLD_H
#define EDUJBARRIOS_AI_STRATIFIED_K_FOLD_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* labels[n] contains 0/1 only, n=2..4096; k>=2 and each class >=k.
   Caller-owned folds[n] must not alias labels and is unchanged on error. */
bool stratified_k_fold(const uint8_t *labels,size_t n,size_t k,uint32_t *folds);
#endif
