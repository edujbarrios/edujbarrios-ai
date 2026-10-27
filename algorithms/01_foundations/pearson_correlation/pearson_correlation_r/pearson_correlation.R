# Pearson product-moment correlation with declared bounded numeric input.
pearson_correlation <- function(x, y) {
  if (!is.numeric(x) || !is.numeric(y) ||
      is.complex(x) || is.complex(y) ||
      !is.null(dim(x)) || !is.null(dim(y)) ||
      length(x) < 2L || length(x) > 10000L || length(y) != length(x) ||
      any(!is.finite(x)) || any(!is.finite(y)) ||
      any(abs(x) > 1000000) || any(abs(y) > 1000000)) {
    stop("invalid paired numerical observations")
  }
  dx <- as.double(x) - mean(x)
  dy <- as.double(y) - mean(y)
  sxx <- sum(dx * dx)
  syy <- sum(dy * dy)
  if (sxx <= 0 || syy <= 0) stop("correlation undefined for constant features")
  r <- (sum(dx * dy) / sqrt(sxx)) / sqrt(syy)
  if (!is.finite(r)) stop("nonfinite correlation")
  max(-1.0, min(1.0, r))
}
