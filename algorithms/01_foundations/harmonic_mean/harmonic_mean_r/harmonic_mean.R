# Harmonic mean via scaled reciprocal ratios, avoiding overflow.
harmonic_mean <- function(values) {
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L || length(values)>512L ||
      any(!is.finite(values)) || any(values<0)) {
    stop("expected 1..512 finite nonnegative observations")
  }
  x <- as.double(values)
  a <- min(x)
  if (a == 0) return(0.0)
  total <- 0.0
  for (value in x) total <- total + a/value
  result <- a/(total/length(x))
  if (!is.finite(result)) stop("nonfinite harmonic mean")
  as.double(result)
}
