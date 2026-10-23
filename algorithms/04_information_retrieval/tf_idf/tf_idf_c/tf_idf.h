#ifndef EDUJBARRIOS_AI_TF_IDF_H
#define EDUJBARRIOS_AI_TF_IDF_H
#include <stdbool.h>
#include <stddef.h>
/* Raw TF multiplied by natural-log unsmoothed IDF; *out unchanged on error. */
bool tf_idf(size_t term_frequency, size_t n_documents,
           size_t document_frequency, double *out);
#endif
