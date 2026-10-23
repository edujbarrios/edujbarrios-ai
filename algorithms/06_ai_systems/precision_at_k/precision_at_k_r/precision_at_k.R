# Precision@K for ordered binary (0/1) relevance judgments.
precision_at_k <- function(relevance, k) {
  if (!is.numeric(relevance) || is.complex(relevance) ||
      length(relevance) == 0L || any(!is.finite(relevance)) ||
      any(!(relevance %in% c(0, 1)))) {
    stop("relevance must be a nonempty binary numeric vector")
  }
  if (!is.numeric(k) || length(k) != 1L || !is.finite(k) ||
      k != floor(k) || k < 1 || k > length(relevance)) {
    stop("k must be an integer in 1..length(relevance)")
  }
  sum(relevance[seq_len(k)]) / k
}
