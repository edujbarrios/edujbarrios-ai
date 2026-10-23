# One-feature min-max feature scaling of finite nonconstant observations.
min_max_scaling <- function(values) {
  if (!is.numeric(values) || is.complex(values) || length(values)==0L ||
      any(!is.finite(values))) {
    stop("input must be a nonempty finite real vector")
  }
  low <- min(values)
  high <- max(values)
  span <- as.double(high) - as.double(low)
  if (!is.finite(span) || span <= 0) {
    stop("input must be nonconstant with representable range")
  }
  output <- numeric(length(values))
  for (i in seq_along(values)) {
    x <- as.double(values[[i]])
    if (x == low) output[[i]] <- 0.0
    else if (x == high) output[[i]] <- 1.0
    else {
      result <- (x - low) / span
      if (!is.finite(result)) stop("normalization overflow")
      output[[i]] <- min(1.0,max(0.0,result))
    }
  }
  output
}
