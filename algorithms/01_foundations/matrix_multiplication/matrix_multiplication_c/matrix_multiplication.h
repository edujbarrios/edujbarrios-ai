#ifndef EDUJBARRIOS_AI_MATRIX_MULTIPLICATION_H
#define EDUJBARRIOS_AI_MATRIX_MULTIPLICATION_H
#include <stdbool.h>
#include <stddef.h>

/* Row-major A[rows_a x shared], B[shared x cols_b], out[rows_a x cols_b].
   Buffers must be allocated to their declared sizes; out must not overlap
   A or B. Outputs are unchanged on failure. */
bool matrix_multiplication(const double *a, size_t rows_a, size_t shared,
                           const double *b, size_t cols_b, double *out);
#endif
