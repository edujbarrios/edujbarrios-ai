# Median absolute error with finite real observations and safe even midpoint.
median_absolute_error <- function(actual, predicted) {
  check <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x))) {
      stop("nonempty finite real vector required")
    }
    as.double(x)
  }
  y <- check(actual)
  prediction <- check(predicted)
  if (length(y) != length(prediction)) stop("lengths must match")
  errors <- abs(y - prediction)
  if (any(!is.finite(errors))) stop("absolute residual overflow")
  errors <- sort(errors)
  n <- length(errors)
  mid <- n %/% 2L
  if (n %% 2L == 1L) return(errors[[mid + 1L]])
  lower <- errors[[mid]]
  upper <- errors[[mid + 1L]]
  lower + (upper - lower) / 2
}
