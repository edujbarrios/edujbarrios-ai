#ifndef BINARY_IOU_H
#define BINARY_IOU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Compute binary segmentation IoU. On failure, *score is unchanged. */
bool binary_iou(const uint8_t *prediction, size_t prediction_len,
                const uint8_t *target, size_t target_len, double *score);

#endif
