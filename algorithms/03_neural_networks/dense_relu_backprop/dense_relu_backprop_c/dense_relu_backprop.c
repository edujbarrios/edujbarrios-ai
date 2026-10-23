#include "dense_relu_backprop.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool dense_relu_backprop(const double *x, size_t d, const double *weights,
                         const double *bias, const double *upstream, size_t o,
                         double *grad_x, double *grad_weights, double *grad_bias) {
    if (!x || !weights || !bias || !upstream || !grad_x ||
        !grad_weights || !grad_bias || d==0 || o==0 ||
        o>SIZE_MAX/d || o*d>SIZE_MAX/sizeof(double) ||
        d>SIZE_MAX/sizeof(double) || o>SIZE_MAX/sizeof(double)) return false;

    for (size_t i=0;i<d;++i) if (!isfinite(x[i])) return false;
    for (size_t i=0;i<o*d;++i) if (!isfinite(weights[i])) return false;
    for (size_t j=0;j<o;++j) {
        if (!isfinite(bias[j]) || !isfinite(upstream[j])) return false;
    }

    double *dx=calloc(d,sizeof(*dx));
    double *dw=malloc(o*d*sizeof(*dw));
    double *db=malloc(o*sizeof(*db));
    if (!dx||!dw||!db) {free(dx);free(dw);free(db);return false;}

    for (size_t j=0;j<o;++j) {
        double z=0.0;
        for (size_t i=0;i<d;++i) {
            const double product=weights[j*d+i]*x[i];
            if (!isfinite(product)) goto fail;
            z+=product;
            if (!isfinite(z)) goto fail;
        }
        z+=bias[j];
        if (!isfinite(z)) goto fail;
        const double dz=z>0.0 ? upstream[j] : 0.0;
        db[j]=dz;
        for (size_t i=0;i<d;++i) {
            const double w_grad=dz*x[i];
            const double term=weights[j*d+i]*dz;
            if (!isfinite(w_grad)||!isfinite(term)) goto fail;
            dw[j*d+i]=w_grad;
            dx[i]+=term;
            if (!isfinite(dx[i])) goto fail;
        }
    }
    memcpy(grad_x,dx,d*sizeof(*dx));
    memcpy(grad_weights,dw,o*d*sizeof(*dw));
    memcpy(grad_bias,db,o*sizeof(*db));
    free(dx);free(dw);free(db);
    return true;
fail:
    free(dx);free(dw);free(db);
    return false;
}
