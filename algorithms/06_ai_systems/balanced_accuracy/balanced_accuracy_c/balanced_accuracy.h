#ifndef EDUJBARRIOS_AI_BALANCED_ACCURACY_H
#define EDUJBARRIOS_AI_BALANCED_ACCURACY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Return false on invalid vectors or missing truth classes; leave *out unchanged. */
bool balanced_accuracy(const uint8_t *truth, const uint8_t *predictions,
                       size_t n, double *out);
#endif
