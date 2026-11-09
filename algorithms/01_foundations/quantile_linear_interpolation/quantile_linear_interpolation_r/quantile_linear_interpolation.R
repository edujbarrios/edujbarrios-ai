# Explicit type-7 sample quantile with linear interpolation.
quantile_linear_interpolation <- function(values, probability) {
  if (!is.numeric(probability) || is.logical(probability) ||
      is.complex(probability) || !is.null(dim(probability)) ||
      length(probability)!=1L || !is.finite(probability) ||
      probability<0 || probability>1) {
    stop("probability must be finite in [0,1]")
  }
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L || length(values)>512L ||
      any(!is.finite(values))) {
    stop("expected 1..512 finite observations")
  }
  sorted <- sort(as.double(values))
  h <- (length(sorted)-1L)*as.double(probability)
  i <- as.integer(floor(h))+1L
  f <- h-floor(h)
  result <- if (f==0) sorted[[i]] else (1-f)*sorted[[i]]+f*sorted[[i+1L]]
  if (!is.finite(result)) stop("nonfinite quantile")
  as.double(result)
}
