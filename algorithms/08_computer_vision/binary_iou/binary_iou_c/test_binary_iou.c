#include "binary_iou.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const uint8_t a[] = {1, 1, 0, 0};
    const uint8_t b[] = {1, 0, 1, 0};
    const uint8_t zeros[] = {0, 0, 0, 0};
    const uint8_t ones[] = {1, 1, 1, 1};
    const uint8_t invalid[] = {0, 0, 2, 0};
    double score = -1.0;

    assert(binary_iou(a, 4, b, 4, &score));
    assert(fabs(score - 1.0 / 3.0) < 1e-12);
    assert(binary_iou(a, 4, a, 4, &score) && score == 1.0);
    assert(binary_iou(zeros, 4, zeros, 4, &score) && score == 1.0);
    assert(binary_iou(zeros, 4, ones, 4, &score) && score == 0.0);
    score = -1.0;
    assert(!binary_iou(a, 4, b, 3, &score) && score == -1.0);
    assert(!binary_iou(a, 0, b, 0, &score) && score == -1.0);
    assert(!binary_iou(a, 4, invalid, 4, &score) && score == -1.0);
    assert(!binary_iou(NULL, 4, b, 4, &score));
    assert(!binary_iou(a, 4, b, 4, NULL));
    puts("C binary IoU tests passed");
    return 0;
}
