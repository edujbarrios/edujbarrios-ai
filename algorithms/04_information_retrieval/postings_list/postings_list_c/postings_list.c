#include "postings_list.h"
#include <stdint.h>

bool postings_list(const int32_t *tokens, size_t n_tokens,
                   const size_t *offsets, size_t n_documents,
                   int32_t target, size_t *doc_ids,
                   size_t *frequencies, size_t *out_count) {
    if (!offsets || !out_count ||
        (n_tokens > 0 && !tokens) ||
        (n_documents > 0 && (!doc_ids || !frequencies)) ||
        n_documents == SIZE_MAX ||
        offsets[0] != 0 ||
        offsets[n_documents] != n_tokens) return false;
    for (size_t i = 0; i < n_documents; ++i) {
        if (offsets[i] > offsets[i+1] || offsets[i+1] > n_tokens) return false;
    }

    size_t count = 0;
    for (size_t document = 0; document < n_documents; ++document) {
        size_t frequency = 0;
        for (size_t j = offsets[document]; j < offsets[document+1]; ++j) {
            if (tokens[j] == target) ++frequency;
        }
        if (frequency > 0) {
            doc_ids[count] = document;
            frequencies[count] = frequency;
            ++count;
        }
    }
    *out_count = count;
    return true;
}
