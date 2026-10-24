#include "momentum_sgd.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define MOMENTUM 0.9

bool momentum_sgd(const double *theta,const double *gradient,
                  const double *velocity,size_t n,double learning_rate,
                  double *theta_out,double *velocity_out) {
    if (!theta || !gradient || !velocity || !theta_out || !velocity_out ||
        n==0 || n>SIZE_MAX/sizeof(double) || !isfinite(learning_rate) ||
        learning_rate<=0.0) return false;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(theta[i]) || !isfinite(gradient[i]) ||
            !isfinite(velocity[i])) return false;
    }
    double *updated=malloc(n*sizeof(*updated));
    double *next_velocity=malloc(n*sizeof(*next_velocity));
    if (!updated || !next_velocity) {
        free(updated);free(next_velocity);return false;
    }
    for (size_t i=0;i<n;++i) {
        const double retained=MOMENTUM*velocity[i];
        next_velocity[i]=retained+gradient[i];
        const double delta=learning_rate*next_velocity[i];
        updated[i]=theta[i]-delta;
        if (!isfinite(retained) || !isfinite(next_velocity[i]) ||
            !isfinite(delta) || !isfinite(updated[i])) {
            free(updated);free(next_velocity);return false;
        }
    }
    memcpy(theta_out,updated,n*sizeof(*updated));
    memcpy(velocity_out,next_velocity,n*sizeof(*next_velocity));
    free(updated);free(next_velocity);
    return true;
}
