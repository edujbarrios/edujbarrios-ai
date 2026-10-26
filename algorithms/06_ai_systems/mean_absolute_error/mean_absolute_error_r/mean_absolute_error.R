# Mean absolute error of finite aligned real regression observations.
mean_absolute_error <- function(truth, predictions) {
  valid <- function(x) is.numeric(x) && !is.complex(x) && all(is.finite(x))
  if (!valid(truth) || !valid(predictions) ||
      length(truth) == 0L || length(truth) != length(predictions)) {
    stop("nonempty aligned finite numeric vectors required")
  }
  residuals <- as.double(truth) - as.double(predictions)
  if (any(!is.finite(residuals))) stop("residual overflow")
  result <- sum(abs(residuals) / length(residuals))
  if (!is.finite(result)) stop("average overflow")
  as.double(result)
}
