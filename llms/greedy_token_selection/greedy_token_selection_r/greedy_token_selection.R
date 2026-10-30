# Return the zero-based ID of the first maximum finite logit.
greedy_token_selection <- function(logits) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits) == 0L || any(!is.finite(logits))) {
    stop("logits must be nonempty finite real numbers")
  }
  as.integer(which.max(logits) - 1L)
}
