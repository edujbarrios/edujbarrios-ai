# Raw median absolute deviation, no Gaussian consistency correction.
median_absolute_deviation <- function(values) {
  if (!is.numeric(values) || is.complex(values) || !is.null(dim(values)) ||
      length(values) < 1L || length(values) > 512L ||
      any(!is.finite(values))) stop("expected 1..512 finite numeric observations")
  median_sorted <- function(sorted) {
    n <- length(sorted)
    if (n %% 2L == 1L) return(sorted[[(n + 1L) %/% 2L]])
    left <- sorted[[n %/% 2L]]
    right <- sorted[[n %/% 2L + 1L]]
    if (left <= 0 && right >= 0) left / 2 + right / 2
    else left + (right - left) / 2
  }
  x <- as.double(values)
  center <- median_sorted(sort(x))
  if (!is.finite(center)) stop("nonfinite center")
  d <- abs(x - center)
  if (any(!is.finite(d))) stop("nonfinite deviation")
  result <- median_sorted(sort(d))
  if (!is.finite(result) || result < 0) stop("nonfinite result")
  as.double(result)
}
