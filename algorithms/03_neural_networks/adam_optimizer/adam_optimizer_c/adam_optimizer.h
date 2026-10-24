#ifndef EDUJBARRIOS_AI_ADAM_OPTIMIZER_H
#define EDUJBARRIOS_AI_ADAM_OPTIMIZER_H
#include <stdbool.h>
#include <stddef.h>
/* n>0, step is 1-based, learning_rate>0.
   Input arrays parameters, gradients, m, v have n entries each.
   p_out, m_out, v_out are separate n-element outputs disjoint from inputs.
   All outputs unchanged if false is returned. */
bool adam_optimizer(const double *parameters, const double *gradients,
                    const double *m, const double *v, size_t n,
                    double learning_rate, size_t step,
                    double *p_out, double *m_out, double *v_out);
#endif
