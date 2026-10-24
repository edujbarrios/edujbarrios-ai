#include "rotary_positional_embedding.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool rotary_positional_embedding(const double *query, const double *key,
    size_t d, size_t query_position, size_t key_position,
    double *rotated_query, double *rotated_key) {
    if (!query || !key || !rotated_query || !rotated_key ||
        d < 2 || d > 1024 || d % 2 != 0 ||
        query_position > 1000000 || key_position > 1000000)
        return false;
    for (size_t j = 0; j < d; ++j) {
        if (!isfinite(query[j]) || !isfinite(key[j])) return false;
    }
    double *temp_q=malloc(d*sizeof(*temp_q));
    double *temp_k=malloc(d*sizeof(*temp_k));
    if (!temp_q || !temp_k) {free(temp_q);free(temp_k);return false;}
    for (size_t j = 0; j < d; j += 2) {
        const double frequency = pow(10000.0, (double)j/(double)d);
        const double theta_q = (double)query_position / frequency;
        const double theta_k = (double)key_position / frequency;
        const double cq=cos(theta_q),sq=sin(theta_q);
        const double ck=cos(theta_k),sk=sin(theta_k);
        temp_q[j]=query[j]*cq-query[j+1]*sq;
        temp_q[j+1]=query[j]*sq+query[j+1]*cq;
        temp_k[j]=key[j]*ck-key[j+1]*sk;
        temp_k[j+1]=key[j]*sk+key[j+1]*ck;
        if (!isfinite(temp_q[j]) || !isfinite(temp_q[j+1]) ||
            !isfinite(temp_k[j]) || !isfinite(temp_k[j+1])) {
            free(temp_q);free(temp_k);return false;
        }
    }
    memcpy(rotated_query,temp_q,d*sizeof(*temp_q));
    memcpy(rotated_key,temp_k,d*sizeof(*temp_k));
    free(temp_q);free(temp_k);
    return true;
}
