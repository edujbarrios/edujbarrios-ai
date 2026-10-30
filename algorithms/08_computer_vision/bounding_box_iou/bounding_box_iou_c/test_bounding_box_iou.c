#include "bounding_box_iou.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double a[4] = {0, 0, 2, 2}, b[4] = {1, 1, 3, 3};
    const double far[4] = {4, 4, 5, 5};
    const double touching[4] = {2, 0, 3, 1};
    const double negative[4] = {-2, -2, 0, 0};
    const double invalid[4] = {1, 1, 1, 2};
    const double nanbox[4] = {0, NAN, 2, 2};
    const double large[4] = {0, 0, 1000001, 2};
    double result = -1;
    assert(bounding_box_iou(a, b, &result) && fabs(result - 1.0/7.0) < 1e-12);
    assert(bounding_box_iou(a, a, &result) && result == 1.0);
    assert(bounding_box_iou(a, far, &result) && result == 0.0);
    assert(bounding_box_iou(a, touching, &result) && result == 0.0);
    assert(bounding_box_iou(a, negative, &result) && result == 0.0);
    result = -1;
    assert(!bounding_box_iou(a, invalid, &result) && result == -1);
    assert(!bounding_box_iou(a, nanbox, &result) && result == -1);
    assert(!bounding_box_iou(a, large, &result) && result == -1);
    assert(!bounding_box_iou(NULL, b, &result) && result == -1);
    assert(!bounding_box_iou(a, b, NULL));
    puts("C bounding box IoU tests passed");
    return 0;
}
