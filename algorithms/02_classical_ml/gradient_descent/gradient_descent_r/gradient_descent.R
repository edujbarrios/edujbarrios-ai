# Fit one-feature linear model using full-batch MSE gradient descent.
gradient_descent <- function(x, y, learning_rate, epochs) {
  if (!is.numeric(x) || !is.numeric(y) || is.complex(x) || is.complex(y) ||
      length(x) == 0L || length(x) != length(y) ||
      any(!is.finite(x)) || any(!is.finite(y))) {
    stop("paired finite real observations required")
  }
  if (!is.numeric(learning_rate) || length(learning_rate) != 1L ||
      !is.finite(learning_rate) || learning_rate <= 0 ||
      !is.numeric(epochs) || length(epochs) != 1L ||
      !is.finite(epochs) || epochs < 1 || epochs != floor(epochs)) {
    stop("invalid learning rate or epoch count")
  }
  weight <- 0.0
  bias <- 0.0
  for (step in seq_len(epochs)) {
    gw <- 0.0
    gb <- 0.0
    for (i in seq_along(x)) {
      prediction <- weight * as.double(x[[i]]) + bias
      error <- prediction - as.double(y[[i]])
      component <- error * as.double(x[[i]])
      if (any(!is.finite(c(prediction,error,component)))) {
        stop("non-finite training intermediate")
      }
      gw <- gw + component
      gb <- gb + error
      if (any(!is.finite(c(gw,gb)))) stop("gradient overflow")
    }
    factor <- 2 * learning_rate / length(x)
    weight <- weight - factor * gw
    bias <- bias - factor * gb
    if (any(!is.finite(c(factor,weight,bias)))) stop("training diverged")
  }
  list(weight=weight,bias=bias)
}
