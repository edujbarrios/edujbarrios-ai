# Full-batch logistic regression for one finite numeric predictor.
logistic_regression <- function(x, y, learning_rate, epochs) {
  if (!is.numeric(x) || !is.numeric(y) || is.complex(x) || is.complex(y) ||
      length(x) == 0L || length(x) != length(y) ||
      any(!is.finite(x)) || any(!is.finite(y)) ||
      any(!(y %in% c(0,1)))) {
    stop("paired finite samples and binary labels required")
  }
  if (!is.numeric(learning_rate) || length(learning_rate) != 1L ||
      !is.finite(learning_rate) || learning_rate <= 0 ||
      !is.numeric(epochs) || length(epochs) != 1L ||
      !is.finite(epochs) || epochs != floor(epochs) || epochs < 1) {
    stop("learning_rate must be positive and epochs a positive integer")
  }
  sigmoid <- function(z) {
    if (z >= 0) {
      e <- exp(-z)
      1 / (1 + e)
    } else {
      e <- exp(z)
      e / (1 + e)
    }
  }
  weight <- 0.0
  bias <- 0.0
  for (epoch in seq_len(epochs)) {
    gw <- 0.0
    gb <- 0.0
    for (i in seq_along(x)) {
      z <- weight * as.double(x[[i]]) + bias
      if (!is.finite(z)) stop("logit overflow")
      error <- sigmoid(z) - as.double(y[[i]])
      component <- error * as.double(x[[i]])
      if (!is.finite(component)) stop("gradient overflow")
      gw <- gw + component
      gb <- gb + error
      if (any(!is.finite(c(gw,gb)))) stop("gradient overflow")
    }
    factor <- learning_rate / length(x)
    dw <- factor * gw
    db <- factor * gb
    if (any(!is.finite(c(factor,dw,db)))) stop("update overflow")
    weight <- weight - dw
    bias <- bias - db
    if (any(!is.finite(c(weight,bias)))) stop("training diverged")
  }
  list(weight = weight, bias = bias)
}
