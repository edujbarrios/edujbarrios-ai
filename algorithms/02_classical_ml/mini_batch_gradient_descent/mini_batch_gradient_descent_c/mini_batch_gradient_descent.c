#include "mini_batch_gradient_descent.h"
#include <math.h>

bool mini_batch_gradient_descent(
    const double *x, const double *y, size_t n,
    double learning_rate, size_t epochs, size_t batch_size,
    double *weight_out, double *bias_out) {
    if (!x || !y || !weight_out || !bias_out || weight_out==bias_out ||
        n==0 || epochs==0 || batch_size==0 || batch_size>n ||
        !isfinite(learning_rate) || learning_rate<=0.0) return false;
    for (size_t i=0; i<n; ++i) {
        if (!isfinite(x[i]) || !isfinite(y[i])) return false;
    }
    double weight=0.0, bias=0.0;
    for (size_t epoch=0; epoch<epochs; ++epoch) {
        for (size_t start=0; start<n; ) {
            const size_t m = n-start < batch_size ? n-start : batch_size;
            double grad_w=0.0, grad_b=0.0;
            for (size_t i=start; i<start+m; ++i) {
                const double prediction=weight*x[i]+bias;
                const double error=prediction-y[i];
                const double contribution=error*x[i];
                if (!isfinite(prediction) || !isfinite(error) ||
                    !isfinite(contribution)) return false;
                grad_w+=contribution;
                grad_b+=error;
                if (!isfinite(grad_w) || !isfinite(grad_b)) return false;
            }
            const double factor=learning_rate/(double)m;
            const double dw=factor*grad_w, db=factor*grad_b;
            if (!isfinite(factor) || !isfinite(dw) || !isfinite(db)) return false;
            weight-=dw; bias-=db;
            if (!isfinite(weight) || !isfinite(bias)) return false;
            start+=m;
        }
    }
    *weight_out=weight; *bias_out=bias;
    return true;
}
