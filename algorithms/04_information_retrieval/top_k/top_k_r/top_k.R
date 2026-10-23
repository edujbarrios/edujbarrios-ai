# Full-sort Top-K indices: descending score, ascending original index.
top_k <- function(scores, k) {
  if (!is.numeric(scores) || is.complex(scores) ||
      length(scores) == 0L || any(!is.finite(scores))) {
    stop("scores must be a nonempty finite real vector")
  }
  if (!is.numeric(k) || is.complex(k) || length(k) != 1L ||
      !is.finite(k) || k != floor(k) || k < 1 || k > length(scores)) {
    stop("k must be an integer between 1 and number of scores")
  }
  indices <- order(-as.double(scores), seq_along(scores))
  as.integer(indices[seq_len(k)] - 1L)
}
