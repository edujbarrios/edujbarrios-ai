#ifndef EDUJBARRIOS_AI_RANDOM_HYPERPLANE_HASH_H
#define EDUJBARRIOS_AI_RANDOM_HYPERPLANE_HASH_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* x[d] and normals[p*d], row-major; 1<=p<=30, d>=1.
   Buffers must have specified capacities. On error *signature unchanged. */
bool random_hyperplane_hash(const double *x, size_t d,
                            const double *normals, size_t p,
                            uint32_t *signature);
#endif
