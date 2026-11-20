#include "precision_recall_curve.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double scores[] = {0.8, 0.8, 0.5, 0.2};
    const uint8_t labels[] = {1, 0, 1, 0};
    const double invalid_scores[] = {0.8, NAN};
    const uint8_t invalid_labels[] = {1, 2}, negative[] = {0, 0};
    const double one_score[] = {1.0};
    const uint8_t one_label[] = {1};
    pr_curve_point points[4] = {{-1.0, -1.0, -1.0}};
    size_t count = 999;
    assert(precision_recall_curve(scores, labels, 4, points, &count));
    assert(count == 3);
    assert(points[0].threshold == 0.8 && points[0].precision == 0.5 &&
           points[0].recall == 0.5);
    assert(points[1].threshold == 0.5 && fabs(points[1].precision - 2.0 / 3.0) < 1e-12 &&
           points[1].recall == 1.0);
    assert(points[2].threshold == 0.2 && points[2].precision == 0.5 &&
           points[2].recall == 1.0);
    assert(precision_recall_curve(one_score, one_label, 1, points, &count));
    assert(count == 1 && points[0].precision == 1.0);
    count = 999;
    points[0].threshold = -1.0;
    assert(!precision_recall_curve(invalid_scores, labels, 2, points, &count));
    assert(!precision_recall_curve(scores, invalid_labels, 2, points, &count));
    assert(!precision_recall_curve(scores, negative, 2, points, &count));
    assert(!precision_recall_curve(scores, labels, 0, points, &count));
    assert(!precision_recall_curve(scores, labels, 4, NULL, &count));
    assert(count == 999 && points[0].threshold == -1.0);
    puts("C precision-recall curve tests passed");
    return 0;
}
