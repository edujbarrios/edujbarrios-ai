# Exponential moving average initialized at the first observation.
exponential_moving_average <- function(values, alpha) {
  if (!is.numeric(values) || is.complex(values) ||
      length(values) == 0L || any(!is.finite(values))) {
    stop("a nonempty finite real vector is required")
  }
  if (!is.numeric(alpha) || is.complex(alpha) || length(alpha) != 1L ||
      !is.finite(alpha) || alpha <= 0 || alpha > 1) {
    stop("alpha must be finite and in (0,1]")
  }
  n <- length(values)
  result <- numeric(n)
  result[1] <- as.double(values[1])
  if (n > 1L) {
    for (i in 2:n) {
      result[i] <- (1 - alpha) * result[i - 1] + alpha * values[i]
      if (!is.finite(result[i])) stop("non-finite EMA")
    }
  }
  result
}
