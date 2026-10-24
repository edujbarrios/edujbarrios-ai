#ifndef EDUJBARRIOS_AI_VITERBI_DECODING_H
#define EDUJBARRIOS_AI_VITERBI_DECODING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Binary observations of length 1..4096, normalized strictly positive
   initial[2], transition[4], emission[4] row-major.
   output states[T] and log_prob remain unchanged on failure;
   caller provides correctly sized, disjoint input/output buffers. */
bool viterbi_decoding(const uint8_t *observations,size_t length,
    const double *initial,const double *transition,const double *emission,
    uint8_t *states,double *log_probability);
#endif
