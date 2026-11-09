#ifndef EDUJBARRIOS_AI_ROOT_MEAN_SQUARE_H
#define EDUJBARRIOS_AI_ROOT_MEAN_SQUARE_H
#include <stdbool.h>
#include <stddef.h>
/* RMS of 1..512 finite doubles. On failure output is unchanged. */
bool root_mean_square(const double *values, size_t n, double *rms_out);
#endif
