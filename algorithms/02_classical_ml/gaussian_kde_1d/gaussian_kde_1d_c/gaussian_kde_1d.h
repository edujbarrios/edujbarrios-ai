#ifndef EDUJBARRIOS_AI_GAUSSIAN_KDE_1D_H
#define EDUJBARRIOS_AI_GAUSSIAN_KDE_1D_H
#include <stdbool.h>
#include <stddef.h>
/* Gaussian kernel density for 1..512 bounded samples at one query point.
   Leaves output unchanged if validation fails. */
bool gaussian_kde_1d(const double *samples, size_t n, double query,
                     double bandwidth, double *output);
#endif
