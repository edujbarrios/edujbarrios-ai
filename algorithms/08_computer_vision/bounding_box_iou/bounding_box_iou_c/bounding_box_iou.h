#ifndef BOUNDING_BOX_IOU_H
#define BOUNDING_BOX_IOU_H
#include <stdbool.h>
/* Each input addresses four doubles: [xmin,ymin,xmax,ymax].
   On invalid input *score is unchanged. */
bool bounding_box_iou(const double *a, const double *b, double *score);
#endif
