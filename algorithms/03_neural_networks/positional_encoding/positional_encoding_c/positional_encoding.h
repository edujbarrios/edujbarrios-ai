#ifndef EDUJBARRIOS_AI_POSITIONAL_ENCODING_H
#define EDUJBARRIOS_AI_POSITIONAL_ENCODING_H
#include <stdbool.h>
#include <stddef.h>
/* Fill caller-allocated row-major output[length * width], with
   1<=length<=4096 and 1<=width<=1024. No writes on bad dimensions. */
bool positional_encoding(size_t length, size_t width, double *output);
#endif
