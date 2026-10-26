# Finite single-output R-squared with explicit constant-target semantics.
r_squared_score <- function(actual, predicted) {
  check <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) < 2L || any(!is.finite(x))) {
      stop("at least two finite real observations required")
    }
    as.double(x)
  }
  y <- check(actual)
  pred <- check(predicted)
  if (length(y) != length(pred)) stop("lengths must match")
  if (all(y == y[[1L]])) return(if (all(y == pred)) 1.0 else 0.0)
  target_scale <- max(abs(y))
  residual_scale <- max(c(target_scale, abs(pred)))
  offsets <- y / target_scale - y[[1L]] / target_scale
  centered_mean <- sum(offsets) / length(offsets)
  variation <- sum((offsets - centered_mean)^2)
  if (variation == 0 || !is.finite(variation)) stop("unrepresentable variation")
  squared_error <- sum((y / residual_scale - pred / residual_scale)^2)
  magnitude <- residual_scale / target_scale
  score <- 1 - (squared_error / variation) * magnitude * magnitude
  if (!is.finite(score)) stop("unrepresentable R-squared")
  score
}
