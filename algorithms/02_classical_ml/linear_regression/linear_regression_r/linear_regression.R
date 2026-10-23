# One-feature OLS fit, using explicit centered sums rather than lm().
linear_regression <- function(x, y) {
  if (!is.numeric(x) || !is.numeric(y) || is.complex(x) || is.complex(y) ||
      length(x) != length(y) || length(x) < 2L ||
      any(!is.finite(x)) || any(!is.finite(y))) {
    stop("at least two paired finite real observations required")
  }
  mean_x <- 0.0
  mean_y <- 0.0
  for (i in seq_along(x)) {
    dx <- as.double(x[[i]]) - mean_x
    dy <- as.double(y[[i]]) - mean_y
    if (any(!is.finite(c(dx, dy)))) stop("mean overflow")
    mean_x <- mean_x + dx / i
    mean_y <- mean_y + dy / i
    if (any(!is.finite(c(mean_x, mean_y)))) stop("mean overflow")
  }
  xx <- 0.0
  xy <- 0.0
  for (i in seq_along(x)) {
    dx <- as.double(x[[i]]) - mean_x
    dy <- as.double(y[[i]]) - mean_y
    squared <- dx * dx
    product <- dx * dy
    if (any(!is.finite(c(dx, dy, squared, product)))) stop("overflow")
    xx <- xx + squared
    xy <- xy + product
    if (any(!is.finite(c(xx, xy)))) stop("overflow")
  }
  if (xx == 0) stop("x must not be constant")
  slope <- xy / xx
  intercept <- mean_y - slope * mean_x
  if (any(!is.finite(c(slope, intercept)))) stop("regression overflow")
  list(slope = slope, intercept = intercept)
}
