#include "adam_optimizer.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool adam_optimizer(const double *parameters, const double *gradients,
                    const double *m, const double *v, size_t n,
                    double learning_rate, size_t step,
                    double *p_out, double *m_out, double *v_out) {
    if (!parameters || !gradients || !m || !v || !p_out || !m_out ||
        !v_out || n == 0 || step == 0 || n > SIZE_MAX / sizeof(double) ||
        !isfinite(learning_rate) || learning_rate <= 0.0) return false;

    const double b1 = 0.9, b2 = 0.999, epsilon = 1e-8;
    const double c1 = -expm1((double)step * log(b1));
    const double c2 = -expm1((double)step * log(b2));
    if (!isfinite(c1) || !isfinite(c2) || c1 <= 0 || c2 <= 0) return false;
    double *next = malloc(3 * n * sizeof(double));
    if (!next) return false;
    double *nm = next + n;
    double *nv = next + 2 * n;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(parameters[i]) || !isfinite(gradients[i]) ||
            !isfinite(m[i]) || !isfinite(v[i]) || v[i] < 0.0) goto fail;
        const double squared = gradients[i] * gradients[i];
        nm[i] = b1*m[i] + (1-b1)*gradients[i];
        nv[i] = b2*v[i] + (1-b2)*squared;
        if (!isfinite(squared) || !isfinite(nm[i]) ||
            !isfinite(nv[i]) || nv[i] < 0.0) goto fail;
        const double mhat = nm[i] / c1;
        const double vhat = nv[i] / c2;
        if (!isfinite(mhat) || !isfinite(vhat) || vhat < 0.0) goto fail;
        const double delta = learning_rate * (mhat / (sqrt(vhat) + epsilon));
        next[i] = parameters[i] - delta;
        if (!isfinite(delta) || !isfinite(next[i])) goto fail;
    }
    memcpy(p_out, next, n * sizeof(double));
    memcpy(m_out, nm, n * sizeof(double));
    memcpy(v_out, nv, n * sizeof(double));
    free(next);
    return true;
fail:
    free(next);
    return false;
}
