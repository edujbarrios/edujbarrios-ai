#ifndef EDUJBARRIOS_AI_MARKOV_BEAM_SEARCH_H
#define EDUJBARRIOS_AI_MARKOV_BEAM_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* V=1..8, T=1..8, B=1..8, positive normalized initial[V] and
   row-major transitions[V*V]. Caller owns tokens[T] and log_probability,
   all disjoint, unchanged on any failure. */
bool markov_beam_search(const double *initial,const double *transition,
    size_t vocabulary,size_t length,size_t width,
    uint8_t *tokens,double *log_probability);
#endif
