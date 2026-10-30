#include "bounding_box_iou.h"
#include <math.h>

static bool valid(const double *box) {
    if (!box) return false;
    for (int i = 0; i < 4; ++i) {
        if (!isfinite(box[i]) || fabs(box[i]) > 1000000.0) return false;
    }
    return box[0] < box[2] && box[1] < box[3];
}

bool bounding_box_iou(const double *a, const double *b, double *score) {
    if (!score || !valid(a) || !valid(b)) return false;
    double width = fmax(0.0, fmin(a[2], b[2]) - fmax(a[0], b[0]));
    double height = fmax(0.0, fmin(a[3], b[3]) - fmax(a[1], b[1]));
    double intersection = width * height;
    double area_a = (a[2] - a[0]) * (a[3] - a[1]);
    double area_b = (b[2] - b[0]) * (b[3] - b[1]);
    *score = intersection / (area_a + area_b - intersection);
    return true;
}
