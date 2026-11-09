#ifndef EDUJBARRIOS_AI_GROUP_NORMALIZATION_H
#define EDUJBARRIOS_AI_GROUP_NORMALIZATION_H
#include <stdbool.h>
#include <stddef.h>
/* Channel-major X[C*S], gamma[C], beta[C], output[C*S].
   C=1..32, S=1..64, G divides C, epsilon finite in (0,1].
   Values finite [-1000,1000]. Caller owns distinct output; invalid
   inputs or nonfinite intermediate results leave output unchanged. */
bool group_normalization(const double *input,size_t channels,size_t spatial_size,
                         size_t groups,const double *gamma,const double *beta,
                         double epsilon,double *output);
#endif
