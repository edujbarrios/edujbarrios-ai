# Mean Huber regression loss with finite positive delta and validation.
huber_loss <- function(actual, predicted, delta = 1) {
  check <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x))) {
      stop("nonempty finite real vector required")
    }
    as.double(x)
  }
  if (!is.numeric(delta) || is.complex(delta) || length(delta) != 1L ||
      !is.finite(delta) || delta <= 0) stop("positive finite delta required")
  y <- check(actual)
  yhat <- check(predicted)
  if (length(y) != length(yhat)) stop("vector lengths differ")
  mean <- 0.0
  for (i in seq_along(y)) {
    error <- abs(y[[i]] - yhat[[i]])
    if (!is.finite(error)) stop("residual overflow")
    value <- if (error <= delta) {
      0.5 * error * error
    } else {
      delta * (error - 0.5 * delta)
    }
    if (!is.finite(value)) stop("loss overflow")
    mean <- mean + (value - mean) / i
  }
  if (!is.finite(mean)) stop("nonfinite mean")
  max(0, mean)
}
