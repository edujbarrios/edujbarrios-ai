#include "matrix_multiplication.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool matrix_multiplication(const double *a, size_t rows_a, size_t shared,
                           const double *b, size_t cols_b, double *out) {
    if (!a || !b || !out || !rows_a || !shared || !cols_b ||
        rows_a > SIZE_MAX / shared || shared > SIZE_MAX / cols_b ||
        rows_a > SIZE_MAX / cols_b ||
        rows_a * cols_b > SIZE_MAX / sizeof(double)) {
        return false;
    }
    for (size_t i = 0; i < rows_a * shared; ++i) {
        if (!isfinite(a[i])) return false;
    }
    for (size_t i = 0; i < shared * cols_b; ++i) {
        if (!isfinite(b[i])) return false;
    }
    size_t length = rows_a * cols_b;
    double *scratch = calloc(length, sizeof(double));
    if (!scratch) return false;

    for (size_t i = 0; i < rows_a; ++i) {
        for (size_t j = 0; j < cols_b; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < shared; ++k) {
                double product = a[i * shared + k] * b[k * cols_b + j];
                if (!isfinite(product)) {
                    free(scratch);
                    return false;
                }
                sum += product;
                if (!isfinite(sum)) {
                    free(scratch);
                    return false;
                }
            }
            scratch[i * cols_b + j] = sum;
        }
    }
    memcpy(out, scratch, length * sizeof(double));
    free(scratch);
    return true;
}
