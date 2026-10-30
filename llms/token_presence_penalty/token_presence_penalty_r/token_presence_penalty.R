# Penalize each historically present token just once (zero-based token IDs).
token_presence_penalty <- function(logits, history, penalty) {
  if (!is.numeric(logits) || is.complex(logits) || length(logits) < 1L ||
      length(logits) > 4096L || any(!is.finite(logits)) ||
      any(logits < -1000 | logits > 1000) ||
      !is.numeric(history) || is.complex(history) || length(history) > 4096L ||
      any(!is.finite(history)) || any(history != floor(history)) ||
      any(history < 0 | history >= length(logits)) ||
      !is.numeric(penalty) || is.complex(penalty) ||
      length(penalty) != 1L || !is.finite(penalty) ||
      penalty < 0 || penalty > 5) stop("invalid logits, token IDs or penalty")
  result <- as.double(logits)
  if (length(history) > 0L) {
    present <- unique(as.integer(history)) + 1L
    result[present] <- result[present] - as.double(penalty)
  }
  result
}
