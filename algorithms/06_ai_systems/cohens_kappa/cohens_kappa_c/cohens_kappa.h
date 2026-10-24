#ifndef EDUJBARRIOS_AI_COHENS_KAPPA_H
#define EDUJBARRIOS_AI_COHENS_KAPPA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* n>=1, first[n] and second[n] binary integer labels.
   Caller owns a disjoint result scalar; unchanged on any error.
   Reject degenerate p_e==1 when kappa is undefined. */
bool cohens_kappa(const uint8_t *first,const uint8_t *second,
                  size_t n,double *result);
#endif
