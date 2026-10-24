#ifndef EDUJBARRIOS_AI_BATCH_NORMALIZATION_H
#define EDUJBARRIOS_AI_BATCH_NORMALIZATION_H
#include <stdbool.h>
#include <stddef.h>
/* Training-mode BN: row-major input/output with n=1..512, d=1..32.
   gamma[d], beta[d] and X[n*d] finite with abs <= 1e6.
   Fixed epsilon=1e-5, population variance. Output disjoint and
   unchanged on invalid inputs. */
bool batch_normalization(const double *x,size_t n,size_t d,
                         const double *gamma,const double *beta,
                         double *output);
#endif
