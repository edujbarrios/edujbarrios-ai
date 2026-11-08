# Symmetric trimmed mean with floor(n*f) sorted values removed per tail.
trimmed_mean <- function(values, trim_fraction) {
  if (!is.numeric(trim_fraction) || is.logical(trim_fraction) ||
      is.complex(trim_fraction) || !is.null(dim(trim_fraction)) ||
      length(trim_fraction)!=1L || !is.finite(trim_fraction) ||
      trim_fraction<0 || trim_fraction>=0.5) stop("invalid trim fraction")
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L ||
      length(values)>512L || any(!is.finite(values))) {
    stop("expected 1..512 finite numeric observations")
  }
  sorted <- sort(as.double(values))
  n <- length(sorted)
  k <- as.integer(floor(n*as.double(trim_fraction)))
  kept <- sorted[seq.int(k+1L,n-k)]
  result <- 0.0
  for (value in kept) {
    result <- result+value/length(kept)
    if (!is.finite(result)) stop("nonfinite trimmed mean")
  }
  as.double(result)
}
