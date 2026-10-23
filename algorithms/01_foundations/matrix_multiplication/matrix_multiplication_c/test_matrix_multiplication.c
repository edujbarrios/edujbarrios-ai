#include "matrix_multiplication.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1, 2, 3, 4, 5, 6};
    const double b[] = {7, 8, 9, 10, 11, 12};
    const double bad[] = {NAN};
    const double huge[] = {1e308};
    const double negative[] = {-3};
    const double scalar[] = {2};
    double out[4] = {-1, -1, -1, -1};
    assert(matrix_multiplication(a, 2, 3, b, 2, out));
    assert(out[0] == 58 && out[1] == 64 &&
           out[2] == 139 && out[3] == 154);
    assert(matrix_multiplication(negative, 1, 1, scalar, 1, out));
    assert(out[0] == -6);
    out[0] = -1;
    assert(!matrix_multiplication(NULL, 1, 1, b, 1, out));
    assert(!matrix_multiplication(a, 0, 3, b, 2, out));
    assert(!matrix_multiplication(bad, 1, 1, scalar, 1, out));
    assert(!matrix_multiplication(huge, 1, 1, huge, 1, out));
    assert(out[0] == -1);
    puts("C matrix multiplication tests passed");
    return 0;
}
