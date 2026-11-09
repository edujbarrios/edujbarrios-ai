#ifndef EDUJBARRIOS_AI_EDMONDS_KARP_MAX_FLOW_H
#define EDUJBARRIOS_AI_EDMONDS_KARP_MAX_FLOW_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* n=2..16 directed integer capacities 0..1000, no self-loops.
   Distinct valid source and sink; unchanged output on invalid input. */
bool edmonds_karp_max_flow(
    const uint16_t *capacities, size_t n, size_t source, size_t sink,
    uint32_t *flow_out);
#endif
