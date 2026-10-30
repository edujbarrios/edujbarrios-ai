# Coordinate-wise L1 proximal soft thresholding.
soft_thresholding <- function(values, threshold) {
  if (!is.numeric(values) || is.complex(values) || length(values)==0L ||
      any(!is.finite(values)) || !is.numeric(threshold) ||
      is.complex(threshold) || length(threshold)!=1L ||
      !is.finite(threshold) || threshold < 0) {
    stop("values must be finite and nonempty; threshold finite and nonnegative")
  }
  x <- as.double(values)
  result <- numeric(length(x))
  result[x > threshold] <- x[x > threshold] - threshold
  result[x < -threshold] <- x[x < -threshold] + threshold
  result
}
