# Mean binary cross-entropy computed directly from logits without sigmoid.
binary_cross_entropy_with_logits <- function(logits, targets) {
  if (!is.numeric(logits) || !is.numeric(targets) ||
      is.complex(logits) || is.complex(targets) ||
      !is.null(dim(logits)) || !is.null(dim(targets)) ||
      length(logits)<1L || length(logits)>10000L ||
      length(logits)!=length(targets) ||
      any(!is.finite(logits)) || any(abs(logits)>1000000) ||
      any(!is.finite(targets)) || any(targets<0) || any(targets>1)) {
    stop("invalid logits or binary/soft targets")
  }
  z <- as.double(logits)
  y <- as.double(targets)
  term <- pmax(z,0) - z*y + log1p(exp(-abs(z)))
  as.double(mean(pmax(0,term)))
}
