#ifndef EDUJBARRIOS_AI_DECISION_STUMP_H
#define EDUJBARRIOS_AI_DECISION_STUMP_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Fit binary stump by exhaustive thresholds from x[n]. On success writes
   finite threshold, right_positive (0/1), and misclassified sample count.
   Caller guarantees buffer capacities and distinct output pointers.
   Outputs remain unchanged on failure. */
bool decision_stump(const double *x, const uint8_t *labels, size_t n,
                    double *threshold_out, uint8_t *right_positive_out,
                    size_t *errors_out);
/* Predict using finite feature and threshold and binary orientation. */
bool decision_stump_predict(double feature, double threshold,
                            uint8_t right_positive, uint8_t *label_out);
#endif
