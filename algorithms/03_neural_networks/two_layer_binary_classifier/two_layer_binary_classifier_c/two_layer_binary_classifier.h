#ifndef EDUJBARRIOS_AI_TWO_LAYER_BINARY_CLASSIFIER_H
#define EDUJBARRIOS_AI_TWO_LAYER_BINARY_CLASSIFIER_H
#include <stdbool.h>
#include <stddef.h>
/* x[n], labels[n] (values 0 or 1), learning_rate>0, epochs>0.
   parameters_out[4] in order (w1,b1,w2,b2), unchanged on error.
   Output buffers do not overlap either input; caller provides capacities. */
bool two_layer_binary_classifier(const double *x, const double *labels,
    size_t n, double learning_rate, size_t epochs, double parameters_out[4]);
/* Predict probability of class one using four finite fitted parameters.
   probability is unchanged on error, and must not alias params. */
bool two_layer_predict(const double params[4], double x, double *probability);
#endif
