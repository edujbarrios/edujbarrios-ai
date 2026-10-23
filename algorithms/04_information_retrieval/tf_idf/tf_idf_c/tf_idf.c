#include "tf_idf.h"
#include <math.h>

bool tf_idf(size_t term_frequency, size_t n_documents,
           size_t document_frequency, double *out) {
    if (!out || n_documents == 0 || document_frequency == 0 ||
        document_frequency > n_documents) return false;
    if (term_frequency == 0 || document_frequency == n_documents) {
        *out = 0.0;
        return true;
    }

    const double numerator = (double)n_documents;
    const double denominator = (double)document_frequency;
    const double idf = log(numerator / denominator);
    const double result = (double)term_frequency * idf;
    if (!isfinite(idf) || idf < 0 || !isfinite(result) || result < 0) {
        return false;
    }
    *out = result;
    return true;
}
