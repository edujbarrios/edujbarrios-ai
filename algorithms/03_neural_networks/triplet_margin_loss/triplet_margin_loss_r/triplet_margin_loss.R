# Euclidean triplet margin ranking loss, without batch reduction.
triplet_margin_loss <- function(anchor, positive, negative, margin) {
  valid <- function(x) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      all(is.finite(x)) && all(abs(x) <= 1000000)
  }
  d <- length(anchor)
  if (!valid(anchor) || !valid(positive) || !valid(negative) ||
      d < 1L || d > 512L || length(positive) != d || length(negative) != d ||
      !is.numeric(margin) || length(margin) != 1L ||
      !is.finite(margin) || margin < 0 || margin > 1000) {
    stop("equal bounded finite numeric vectors and a valid margin required")
  }
  dp <- sqrt(sum((anchor - positive)^2))
  dn <- sqrt(sum((anchor - negative)^2))
  max(0, dp - dn + margin)
}
