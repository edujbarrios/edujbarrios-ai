# Raw TF times natural-log unsmoothed inverse document frequency.
tf_idf <- function(term_frequency, n_documents, document_frequency) {
  values <- c(term_frequency, n_documents, document_frequency)
  if (!is.numeric(values) || length(values) != 3L ||
      any(!is.finite(values)) || any(values != floor(values)) ||
      any(values < 0) || any(values > 2^53) ||
      n_documents < 1 || document_frequency < 1 ||
      document_frequency > n_documents) {
    stop("invalid integer term/corpus counts")
  }
  if (term_frequency == 0 || document_frequency == n_documents) return(0.0)
  idf <- log(n_documents / document_frequency)
  weight <- term_frequency * idf
  if (!is.finite(weight) || weight < 0) stop("non-finite TF-IDF weight")
  as.double(weight)
}
