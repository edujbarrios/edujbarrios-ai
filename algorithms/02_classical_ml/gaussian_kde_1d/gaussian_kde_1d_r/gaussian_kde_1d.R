# Gaussian kernel density estimation at one query point, using base R.
gaussian_kde_1d <- function(samples, query, bandwidth) {
  scalar <- function(x, bound) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x) == 1L && is.finite(x) && abs(x) <= bound
  }
  if (!is.numeric(samples) || is.complex(samples) || !is.null(dim(samples)) ||
      length(samples) < 1L || length(samples) > 512L ||
      any(!is.finite(samples)) || any(abs(samples) > 1000) ||
      !scalar(query, 1000) || !scalar(bandwidth, 1000) ||
      bandwidth < 0.001) {
    stop("1..512 bounded observations, bounded query and positive bandwidth required")
  }
  z <- (query - samples) / bandwidth
  sum(exp(-0.5 * z * z)) / (length(samples) * bandwidth * sqrt(2*pi))
}
