# Welford's one-pass mean and population variance for finite observations.
mean_variance <- function(values) {
  if (!is.numeric(values) || is.complex(values) || length(values) == 0L) {
    stop("observations must be nonempty real numbers")
  }
  if (any(!is.finite(values))) stop("observations must be finite")
  mean <- 0.0
  m2 <- 0.0
  for (i in seq_along(values)) {
    x <- as.double(values[[i]])
    delta <- x - mean
    if (!is.finite(delta)) stop("intermediate overflow")
    mean <- mean + delta / i
    delta2 <- x - mean
    increment <- delta * delta2
    if (any(!is.finite(c(mean, delta2, increment)))) stop("intermediate overflow")
    m2 <- m2 + increment
    if (!is.finite(m2)) stop("variance overflow")
  }
  variance <- m2 / length(values)
  if (!is.finite(variance) || variance < 0) stop("variance is not finite")
  list(mean = mean, variance = variance)
}
