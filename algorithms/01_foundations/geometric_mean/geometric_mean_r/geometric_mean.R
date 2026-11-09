# Geometric mean of finite nonnegative observations via logarithms.
geometric_mean <- function(values) {
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L || length(values)>512L ||
      any(!is.finite(values)) || any(values<0)) {
    stop("expected 1..512 finite nonnegative numeric observations")
  }
  x <- as.double(values)
  if (any(x==0)) return(0.0)
  log_sum <- 0.0
  for (value in x) {
    log_sum <- log_sum + log(value)
    if (!is.finite(log_sum)) stop("nonfinite log sum")
  }
  result <- exp(log_sum / length(x))
  if (!is.finite(result)) stop("nonfinite geometric mean")
  as.double(result)
}
