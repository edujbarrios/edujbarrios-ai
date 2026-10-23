# Stable softmax of a nonempty vector of finite real logits.
softmax <- function(logits) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits) == 0L || any(!is.finite(logits))) {
    stop("logits must be nonempty finite real numbers")
  }
  shifted <- as.double(logits) - max(logits)
  weights <- exp(shifted)
  total <- sum(weights)
  if (!is.finite(total) || total <= 0) stop("invalid softmax normalizer")
  weights / total
}
