#ifndef EDUJBARRIOS_AI_PERCEPTRON_H
#define EDUJBARRIOS_AI_PERCEPTRON_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Train an online perceptron from row-major X[n*d], labels[n].
   weights_out[d], bias_out are caller-owned and unchanged on failure.
   All input/output buffers must have capacities and not overlap. */
bool perceptron(const double *features, const uint8_t *labels,
                size_t n, size_t d, double learning_rate, size_t epochs,
                double *weights_out, double *bias_out);
/* Infer one binary label; result unchanged on invalid inputs. */
bool perceptron_predict(const double *weights, const double *features,
                        size_t d, double bias, uint8_t *prediction_out);
#endif
