#ifndef EDUJBARRIOS_AI_OTSU_THRESHOLD_H
#define EDUJBARRIOS_AI_OTSU_THRESHOLD_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* 1..4096 unsigned 8-bit pixels. Writes a threshold 0..254.
   On failure leaves *threshold unchanged. */
bool otsu_threshold(const uint8_t *pixels, size_t length, uint8_t *threshold);
#endif
