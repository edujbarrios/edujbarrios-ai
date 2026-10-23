#ifndef BINARY_CROSS_ENTROPY_H
#define BINARY_CROSS_ENTROPY_H
#include <stdbool.h>
#include <stddef.h>
bool binary_cross_entropy(const double *labels,const double *probabilities,size_t n,double *loss);
#endif
