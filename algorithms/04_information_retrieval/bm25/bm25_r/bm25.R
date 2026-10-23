# Robertson-style BM25 contribution of one query term.
bm25 <- function(n_documents, doc_frequency, term_frequency, document_length,
                 avg_document_length, k1=1.2, b=0.75) {
  counts <- c(n_documents, doc_frequency, term_frequency, document_length)
  parameters <- c(avg_document_length, k1, b)
  if (!is.numeric(counts) || !is.numeric(parameters) ||
      length(counts) != 4L || length(parameters) != 3L ||
      any(!is.finite(counts)) || any(!is.finite(parameters)) ||
      any(counts < 0) || any(counts != floor(counts)) ||
      any(counts > 2^53) || n_documents < 1 ||
      doc_frequency > n_documents || term_frequency > document_length ||
      avg_document_length <= 0 || k1 <= 0 || b < 0 || b > 1) {
    stop("invalid counts or BM25 parameters")
  }
  if (term_frequency == 0) return(0.0)

  length_ratio <- document_length / avg_document_length
  normalization <- k1 * (1 - b + b * length_ratio)
  idf <- log1p((n_documents - doc_frequency + 0.5) / (doc_frequency + 0.5))
  numerator <- idf * term_frequency * (k1 + 1)
  denominator <- term_frequency + normalization
  score <- numerator / denominator
  if (any(!is.finite(c(length_ratio, normalization, idf, numerator,
                       denominator, score))) || denominator <= 0 ||
      score < 0) stop("non-finite BM25 score")
  as.double(score)
}
