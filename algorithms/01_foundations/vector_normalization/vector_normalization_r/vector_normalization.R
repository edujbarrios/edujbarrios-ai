# Stable L2 normalization. Requires a nonempty finite real vector.
vector_normalization <- function(values) {
  if (!is.numeric(values) || is.complex(values) || length(values) == 0L) {
    stop("input must be a nonempty real numeric vector")
  }
  if (any(!is.finite(values))) stop("components must be finite")
  scale <- max(abs(values))
  if (scale == 0) stop("cannot normalize a zero vector")
  scaled <- as.double(values) / scale
  norm <- sqrt(sum(scaled * scaled))
  if (!is.finite(norm) || norm == 0) stop("invalid norm")
  scaled / norm
}
