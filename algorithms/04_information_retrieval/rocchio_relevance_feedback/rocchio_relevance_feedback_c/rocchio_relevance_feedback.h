#ifndef EDUJBARRIOS_AI_ROCCHIO_RELEVANCE_FEEDBACK_H
#define EDUJBARRIOS_AI_ROCCHIO_RELEVANCE_FEEDBACK_H
#include <stdbool.h>
#include <stddef.h>
/* Query width d=1..64; group row counts each <=128, total >=1.
   Documents are row-major flattened matrices, NULL allowed for zero
   rows. All entries finite in [0,1e6]. Output[d] disjoint from all
   inputs and unchanged on error. */
bool rocchio_relevance_feedback(const double *query,size_t d,
    const double *relevant,size_t nr,const double *nonrelevant,size_t ni,
    double *output);
#endif
