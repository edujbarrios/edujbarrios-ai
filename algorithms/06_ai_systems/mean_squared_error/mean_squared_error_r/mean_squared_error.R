# Unweighted mean squared error with finite-input validation.
mean_squared_error <- function(actual, predicted) {
  check <- function(values) {
    if (!is.numeric(values) || is.complex(values) || !is.null(dim(values)) ||
        length(values) == 0L || any(!is.finite(values))) {
      stop("nonempty finite real vector required")
    }
    as.double(values)
  }
  y <- check(actual)
  pred <- check(predicted)
  if (length(y) != length(pred)) stop("vector lengths must match")
  average <- 0
  for (i in seq_along(y)) {
    difference <- y[[i]] - pred[[i]]
    squared <- difference * difference
    if (!is.finite(squared)) stop("squared residual overflow")
    average <- average + (squared - average) / i
  }
  if (!is.finite(average)) stop("nonfinite mean squared error")
  max(0, average)
}
