# Multiclass cross-entropy on finite logits; zero-based target index.
cross_entropy <- function(logits, target) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits)==0L || any(!is.finite(logits)) ||
      !is.numeric(target) || length(target)!=1L ||
      !is.finite(target) || target!=floor(target) ||
      target<0 || target>=length(logits)) {
    stop("invalid logits or target")
  }
  maximum <- max(logits)
  total <- sum(exp(as.double(logits) - maximum))
  delta <- as.double(logits[[target+1L]]) - maximum
  result <- log(total) - delta
  if (any(!is.finite(c(total, delta, result)))) stop("loss not representable")
  max(0.0, result)
}
