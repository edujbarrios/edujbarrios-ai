# Stable log-softmax of a bounded finite numeric vector.
log_softmax <- function(logits) {
  if (!is.numeric(logits) || is.complex(logits) || !is.null(dim(logits)) ||
      length(logits)<1L || length(logits)>10000L ||
      any(!is.finite(logits)) || any(abs(logits)>1000000)) {
    stop("finite bounded numeric logits required")
  }
  x <- as.double(logits)
  maximum <- max(x)
  z <- x-maximum
  correction <- log(sum(exp(z)))
  as.double(z-correction)
}
