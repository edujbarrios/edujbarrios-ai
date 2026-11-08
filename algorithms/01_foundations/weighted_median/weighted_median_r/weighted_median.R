# Lower weighted median of 1..512 finite observations with nonnegative weights.
weighted_median <- function(values, weights) {
  valid <- function(v) {
    is.numeric(v) && !is.logical(v) && !is.complex(v) &&
      is.null(dim(v)) && length(v) >= 1L && length(v) <= 512L &&
      all(is.finite(v))
  }
  if (!valid(values) || !valid(weights) || length(values) != length(weights) ||
      any(weights < 0)) stop("expected equal-length finite values and nonnegative weights")
  ord <- order(values)
  x <- as.double(values[ord])
  w <- as.double(weights[ord])
  total <- 0.0
  for (weight in w) {
    total <- total + weight
    if (!is.finite(total)) stop("weight sum overflow")
  }
  if (total <= 0.0) stop("positive weight sum required")
  cumulative <- 0.0
  for (i in seq_along(x)) {
    cumulative <- cumulative + w[[i]]
    if (cumulative >= total - cumulative) return(as.double(x[[i]]))
  }
  stop("invalid weight sum")
}
