# Sparsemax projection of finite nonempty logits.
sparsemax <- function(logits) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits) == 0L || any(!is.finite(logits))) {
    stop("logits must be nonempty finite real numbers")
  }
  shifted <- as.double(logits) - max(logits)
  sorted <- sort(shifted, decreasing = TRUE)
  prefix <- cumsum(sorted)
  k <- seq_along(sorted)
  support <- which(1 + k * sorted > prefix)
  threshold <- (sum(sorted[seq_len(max(support))]) - 1) / max(support)
  pmax(shifted - threshold, 0)
}
