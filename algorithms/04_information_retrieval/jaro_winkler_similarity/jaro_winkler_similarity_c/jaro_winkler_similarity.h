#ifndef EDUJBARRIOS_AI_JARO_WINKLER_SIMILARITY_H
#define EDUJBARRIOS_AI_JARO_WINKLER_SIMILARITY_H

#include <stdbool.h>

/* Case-sensitive printable ASCII strings, maximum 512 characters each.
   Returns false on invalid input; score_out remains unchanged on failure. */
bool jaro_winkler_similarity(const char *a, const char *b, double *score_out);

#endif
