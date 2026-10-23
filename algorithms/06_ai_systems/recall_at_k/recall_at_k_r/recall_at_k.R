# Recall at k for a nonempty ranking of binary labels.
recall_at_k <- function(relevance, k, total_relevant) {
  if (!is.numeric(relevance) || is.complex(relevance) ||
      length(relevance) == 0L || any(!is.finite(relevance)) ||
      any(!(relevance %in% c(0,1)))) {
    stop("relevance must be a nonempty binary numeric vector")
  }
  if (!is.numeric(k) || is.complex(k) || length(k) != 1L ||
      !is.finite(k) || k != floor(k) || k < 1 || k > length(relevance)) {
    stop("k must be an integer in 1..length(relevance)")
  }
  if (!is.numeric(total_relevant) || is.complex(total_relevant) ||
      length(total_relevant) != 1L || !is.finite(total_relevant) ||
      total_relevant != floor(total_relevant) || total_relevant < 1 ||
      total_relevant < sum(relevance)) {
    stop("total_relevant must be a valid positive integer")
  }
  as.double(sum(relevance[seq_len(k)]) / total_relevant)
}
