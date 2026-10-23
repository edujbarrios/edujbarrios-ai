#include "bm25.h"
#include <math.h>
bool bm25(size_t n_documents, size_t doc_frequency, size_t term_frequency,
          size_t document_length, double avg_document_length,
          double k1, double b, double *score) {
    if (!score || n_documents == 0 || doc_frequency > n_documents ||
        term_frequency > document_length ||
        !isfinite(avg_document_length) || avg_document_length <= 0 ||
        !isfinite(k1) || k1 <= 0 ||
        !isfinite(b) || b < 0 || b > 1) return false;

    if (term_frequency == 0) {
        *score = 0.0;
        return true;
    }

    const double length_ratio = (double)document_length / avg_document_length;
    const double normalization = k1 * (1.0 - b + b * length_ratio);
    const double idf = log1p(((double)(n_documents - doc_frequency) + 0.5) /
                             ((double)doc_frequency + 0.5));
    const double tf = (double)term_frequency;
    const double numerator = idf * tf * (k1 + 1.0);
    const double denominator = tf + normalization;
    const double result = numerator / denominator;
    if (!isfinite(length_ratio) || !isfinite(normalization) ||
        !isfinite(idf) || !isfinite(numerator) ||
        !isfinite(denominator) || denominator <= 0.0 ||
        !isfinite(result) || result < 0.0) return false;
    *score = result;
    return true;
}
