#include "median_absolute_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double odd[] = {0, 10, 0}, zeros[] = {0, 0, 0, 0};
    const double even[] = {1, 2, 3, 4}, same[] = {1, 2};
    const double large[] = {1e308, 1e308};
    const double negative[] = {-1e308}, positive[] = {1e308};
    const double invalid[] = {NAN};
    double score = -9;
    assert(median_absolute_error(odd, zeros, 3, &score) && score == 0);
    assert(median_absolute_error(even, zeros, 4, &score) && score == 2.5);
    assert(median_absolute_error(same, same, 2, &score) && score == 0);
    assert(median_absolute_error(large, zeros, 2, &score) && score == 1e308);
    score = -9;
    assert(!median_absolute_error(odd, zeros, 0, &score));
    assert(!median_absolute_error(NULL, zeros, 3, &score));
    assert(!median_absolute_error(odd, zeros, 3, NULL));
    assert(!median_absolute_error(invalid, zeros, 1, &score));
    assert(!median_absolute_error(positive, negative, 1, &score));
    assert(score == -9);
    puts("C median absolute error tests passed");
    return 0;
}
