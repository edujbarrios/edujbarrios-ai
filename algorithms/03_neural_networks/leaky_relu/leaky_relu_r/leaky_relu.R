# Elementwise Leaky ReLU with 0 < alpha < 1.
leaky_relu <- function(values, alpha) {
  if (!is.numeric(values) || is.complex(values) || any(!is.finite(values))) {
    stop("components must be finite real numbers")
  }
  if (!is.numeric(alpha) || is.complex(alpha) || length(alpha) != 1L ||
      !is.finite(alpha) || alpha <= 0 || alpha >= 1) {
    stop("alpha must be finite and in (0, 1)")
  }
  result <- as.double(values)
  negatives <- result < 0
  result[negatives] <- result[negatives] * alpha
  result
}
