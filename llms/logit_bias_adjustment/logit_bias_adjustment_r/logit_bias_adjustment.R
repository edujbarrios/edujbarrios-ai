# Add sparse per-token biases to finite logits (zero-based IDs).
logit_bias_adjustment <- function(logits, token_ids, biases) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits) < 1L || length(logits) > 4096L ||
      any(!is.finite(logits)) || any(abs(logits) > 1000) ||
      !is.numeric(token_ids) || is.complex(token_ids) ||
      !is.numeric(biases) || is.complex(biases) ||
      length(token_ids) != length(biases) ||
      length(token_ids) > length(logits) ||
      any(!is.finite(token_ids)) || any(token_ids != floor(token_ids)) ||
      any(token_ids < 0 | token_ids >= length(logits)) ||
      anyDuplicated(token_ids) > 0L ||
      any(!is.finite(biases)) || any(abs(biases) > 100)) {
    stop("invalid logits, token IDs or biases")
  }
  result <- as.double(logits)
  if (length(token_ids) > 0L) {
    index <- as.integer(token_ids) + 1L
    result[index] <- result[index] + as.double(biases)
  }
  result
}
