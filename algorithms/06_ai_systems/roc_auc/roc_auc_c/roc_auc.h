#ifndef EDUJBARRIOS_AI_ROC_AUC_H
#define EDUJBARRIOS_AI_ROC_AUC_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Finite scores and binary labels, n>=2, with both classes present.
   Caller-owned output scalar unchanged on failure. Input and output
   memory must not alias. */
bool roc_auc(const double *scores,const uint8_t *labels,size_t n,double *auc);
#endif
