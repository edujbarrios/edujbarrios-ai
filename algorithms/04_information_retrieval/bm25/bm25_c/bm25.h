#ifndef EDUJBARRIOS_AI_BM25_H
#define EDUJBARRIOS_AI_BM25_H
#include <stdbool.h>
#include <stddef.h>
/* One BM25 term contribution; on failure *score is unchanged. */
bool bm25(size_t n_documents, size_t doc_frequency, size_t term_frequency,
          size_t document_length, double avg_document_length,
          double k1, double b, double *score);
#endif
