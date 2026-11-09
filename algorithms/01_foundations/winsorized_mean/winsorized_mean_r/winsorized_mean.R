# Symmetric winsorized mean: clamp k observations per sorted tail.
winsorized_mean <- function(values, trim_fraction) {
  if (!is.numeric(trim_fraction) || is.logical(trim_fraction) ||
      is.complex(trim_fraction) || !is.null(dim(trim_fraction)) ||
      length(trim_fraction)!=1L || !is.finite(trim_fraction) ||
      trim_fraction<0 || trim_fraction>=0.5) {
    stop("invalid trim fraction")
  }
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L || length(values)>512L ||
      any(!is.finite(values))) {
    stop("expected 1..512 finite real observations")
  }
  x <- sort(as.double(values))
  n <- length(x)
  k <- as.integer(floor(n*as.double(trim_fraction)))
  low <- x[[k+1L]]
  high <- x[[n-k]]
  result <- 0.0
  for (i in seq_len(n)) {
    clipped <- if (i<=k) low else if (i>n-k) high else x[[i]]
    result <- result+clipped/n
    if (!is.finite(result)) stop("nonfinite winsorized mean")
  }
  as.double(result)
}
